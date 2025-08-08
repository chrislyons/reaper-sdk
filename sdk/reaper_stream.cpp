#include "reaper_stream.h"
#undef min
#undef max
#include <string>
#include <cstring>
#include <cstdio>
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <unistd.h>
#endif

struct ReaperStreamInternal {
  int sock;
};

static bool parse_url(const char* url, std::string& host, std::string& port, bool& udp)
{
  if (!url) return false;
  if (!strncmp(url, "ws://", 5)) { udp = false; url += 5; }
  else if (!strncmp(url, "srt://", 6)) { udp = true; url += 6; }
  else return false;
  const char* colon = strchr(url, ':');
  if (!colon) return false;
  host.assign(url, colon - url);
  port.assign(colon + 1);
  return true;
}

REAPER_STREAM stream_open(const char* url)
{
#ifdef _WIN32
  static bool wsa_init=false;
  if(!wsa_init){ WSADATA w; WSAStartup(MAKEWORD(2,2), &w); wsa_init=true; }
#endif
  std::string host, port; bool udp=false;
  if(!parse_url(url, host, port, udp)) return nullptr;
  addrinfo hints{}; addrinfo* res=nullptr;
  hints.ai_socktype = udp ? SOCK_DGRAM : SOCK_STREAM;
  hints.ai_family = AF_UNSPEC;
  if(getaddrinfo(host.c_str(), port.c_str(), &hints, &res)!=0) return nullptr;
  int sock = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sock<0){ freeaddrinfo(res); return nullptr; }
  if(connect(sock, res->ai_addr, res->ai_addrlen)<0){
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    freeaddrinfo(res);
    return nullptr;
  }
  freeaddrinfo(res);
  ReaperStreamInternal* s = new ReaperStreamInternal();
  s->sock = sock;
  return (REAPER_STREAM)s;
}

bool stream_send(REAPER_STREAM handle, PCM_source_transfer_t* block)
{
  if(!handle || !block) return false;
  ReaperStreamInternal* s = (ReaperStreamInternal*)handle;
  int bytes = block->samples_out * block->nch * sizeof(ReaSample);
  const char* p = (const char*)block->samples;
  int sent=0;
  while(sent<bytes){
    int rv = send(s->sock, p+sent, bytes-sent, 0);
    if(rv<=0) return false;
    sent+=rv;
  }
  return true;
}

bool stream_receive(REAPER_STREAM handle, PCM_source_transfer_t* block)
{
  if(!handle || !block) return false;
  ReaperStreamInternal* s = (ReaperStreamInternal*)handle;
  int bytes = block->length * block->nch * sizeof(ReaSample);
  char* p = (char*)block->samples;
  int rv = recv(s->sock, p, bytes, 0);
  if(rv<=0) return false;
  block->samples_out = rv / (block->nch * sizeof(ReaSample));
  return true;
}

void stream_close(REAPER_STREAM handle)
{
  if(!handle) return;
  ReaperStreamInternal* s = (ReaperStreamInternal*)handle;
#ifdef _WIN32
  closesocket(s->sock);
#else
  close(s->sock);
#endif
  delete s;
}

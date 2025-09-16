# Cockos WDL SDK

The Cockos Windows Development Library (WDL) is a grab bag of portable C and C++ building blocks that power REAPER and other Cockos products. Rather than being a monolithic framework, the SDK provides mix-and-match modules that you can embed directly into your own projects to add audio DSP, graphics, networking, scripting, and utility infrastructure.

## Repository structure and build approach

WDL intentionally ships without a standalone build system or packaged binaries. Each subsystem under `WDL/` is designed to be compiled as part of your application's existing project files or build scripts, letting you cherry-pick only the components you need. Typical integration involves adding the relevant source and header files to your build, defining any configuration macros required by that module, and wiring it into your application's initialization code.

## Major subsystems

The key subsystems live under the `WDL/` tree and can be pulled in independently:

- **Base types and containers** – Low-level portability helpers, integer typedefs, and utility classes for strings, buffers, and lists live in headers such as [`WDL/wdltypes.h`](WDL/wdltypes.h) and [`WDL/wdlstring.h`](WDL/wdlstring.h).【F:WDL/wdltypes.h†L1-L120】【F:WDL/wdlstring.h†L1-L120】
- **LICE graphics** – The Lightweight Image Compositing Engine exposes bitmaps, drawing primitives, and image import helpers through [`WDL/lice/lice.h`](WDL/lice/lice.h).【F:WDL/lice/lice.h†L1-L120】
- **SWELL UI layer** – SWELL (Simple/Small Win32 Emulation Layer) provides Win32-style APIs on macOS, Linux, and other targets via [`WDL/swell/swell.h`](WDL/swell/swell.h).【F:WDL/swell/swell.h†L1-L116】
- **Virtual window framework** – The virtual window layer and custom controls live in [`WDL/wingui/virtwnd.h`](WDL/wingui/virtwnd.h), enabling lightweight composited UI widgets.【F:WDL/wingui/virtwnd.h†L1-L120】
- **Project context utilities** – Serialization helpers, configuration encoders, and project file abstractions are exposed in [`WDL/projectcontext.h`](WDL/projectcontext.h).【F:WDL/projectcontext.h†L1-L105】
- **Audio DSP components** – Core DSP helpers include the resampler ([`WDL/resample.h`](WDL/resample.h)), FFT wrapper ([`WDL/fft.h`](WDL/fft.h)), and fast convolution engine ([`WDL/convoengine.h`](WDL/convoengine.h)) for implementing high-quality audio processing pipelines.【F:WDL/resample.h†L1-L107】【F:WDL/fft.h†L1-L77】【F:WDL/convoengine.h†L1-L120】
- **NS-EEL scripting** – The embeddable Nullsoft Expression Evaluator runtime is provided via [`WDL/eel2/ns-eel.h`](WDL/eel2/ns-eel.h) for hosting fast, sandboxed scripting inside your application.【F:WDL/eel2/ns-eel.h†L1-L160】
- **JNetLib networking** – Cross-platform TCP, HTTP, and DNS helpers ship in [`WDL/jnetlib/jnetlib.h`](WDL/jnetlib/jnetlib.h) and its companion headers for easy client/server networking.【F:WDL/jnetlib/jnetlib.h†L1-L47】

## Trying the SWELL sample project

A minimal cross-platform GUI starter project is available under `WDL/swell/sample_project`. Before building it, copy the template next to (not inside) your WDL checkout so that the project and the `WDL/` tree become siblings, for example:

```bash
cp -a ./WDL/swell/sample_project ./MySwellApp
```

The sample's `readme.txt` reiterates this requirement and includes historical Xcode setup notes for macOS builds.【F:WDL/swell/sample_project/readme.txt†L21-L65】Once copied, open the new directory in your IDE of choice, add the SWELL sources listed in the template to your project, run the `swell_resgen` resource generator as needed, and link against the `WDL/` sources from the adjacent tree.

## Bundled third-party libraries

Several upstream libraries are vendored under `WDL/` for convenience. Each directory preserves the original documentation and licenses so you can review upstream guidance and track updates:

- **libpng (`WDL/libpng`)** – Contains libpng 1.6.19 sources along with the upstream README and licensing terms (`README`, `LICENSE`, and `png.h`).【F:WDL/libpng/README†L1-L10】【F:WDL/libpng/LICENSE†L2-L17】
- **giflib (`WDL/giflib`)** – Ships giflib 4.1.2 with README notes (including patent caveats) and the permissive license in `COPYING`.【F:WDL/giflib/README†L1-L19】【F:WDL/giflib/COPYING†L1-L19】
- **zlib (`WDL/zlib`)** – Provides zlib 1.3.1 sources plus upstream documentation describing the format, usage examples, and maintenance expectations (`README`, `ChangeLog`, MiniZip notes).【F:WDL/zlib/README†L1-L38】

Refer to those upstream files for the most accurate licensing language and integration guidance when you incorporate the third-party components.

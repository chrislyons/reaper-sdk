## Pristine Fork Policy
- This fork tracks upstream's default branch **exactly**.
- Archive snapshot retained:
  - tag: `v-orph-pre-split`
  - branch: `archive/orph-pre-split`
- Resync recipe:
  ```
  git fetch upstream && git checkout main && git reset --hard upstream/main && git push origin --force
  ```

### Manual action required (insufficient API perms)
Apply Branch Protection to `main`:
- Require a pull request before merging (1 review)
- Require conversation resolution
- Require linear history
- Disallow force pushes and deletions
- (Enable status checks after CI has run once)

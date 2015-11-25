char *extractStringFromQuotes(const char* src, char *dest) {
  if (!*src) {
    *dest = 0;
    return NULL;
  }
  char *p1, *p2;
  if (!(p1 = strchr(src, '\"'))) return 0;
  if (!(p2 = strchr(p1+1, '\"'))) return 0;
  int len = std::min(p2-p1, 19);   //max 20 chars extracted
  strncpy(dest, p1+1, len-1);
  dest[len-1] = 0;
  return p2 + 1;
}

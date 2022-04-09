
#ifndef _DEBUG_PRINT_H__
#define _DEBUG_PRINT_H__

#ifndef DEBUG_PRINT_DISABLE
#define DEBUG_PRINT_DISABLE 0
#endif

#if DEBUG_PRINT_DISABLE
  #define DEBUG_PRINT(...) { }
  #define DEBUG_PRINTLN(...) { }
  #define DEBUG_PRINTF(fmt, args...) { }
#else
  #define DEBUG_PRINTER Serial
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
  #define DEBUG_PRINTF(fmt, args...) { DEBUG_PRINTER.printf(fmt,## args); }
#endif

#endif
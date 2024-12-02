# Attention
Initialize structs: `filter_avr_pull_t = {3}`  
**Undefined behavior**: not initialized struct may lead to unexpected values

# API
All structs have docs for field that need to configure.

# Example
See [example/main.c](./example/main.c)

# Problems
- ? `sqrt` function implemented in lib and access by -lm option. Has STM32 sqrt implementation? 

# Thankings
[*Alex Gyver*](https://alexgyver.ru/lessons/filters/). Some codes for filters' algorithms were copied and rewritten

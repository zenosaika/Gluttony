# Gluttony

### Installation
```
brew install gtk4

gcc $(pkg-config --cflags gtk4) -o Main main.c $(pkg-config --libs gtk4)
```
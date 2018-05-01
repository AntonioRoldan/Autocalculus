CC := g++
CFLAGS := -std=c++11
TARGET := autocalculus

SRCDIR   = src
OBJDIR   = obj

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

directories: $(OBJDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -f $(TARGET)
	rm -rf obj/
	
.PHONY: all clean


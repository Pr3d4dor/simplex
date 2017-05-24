src = $(wildcard *.c)
obj = $(src:.c=.o)

simplex: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
clean:
	rm -f $(obj) simplex
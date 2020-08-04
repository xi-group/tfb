GZIP		=	gzip

TFB_MANPAGE	=	tfb.1

all: $(MANPAGE)
	$(GZIP) < $(TFB_MANPAGE) > $(TFB_MANPAGE).gz

clean:
	rm -rf *.gz

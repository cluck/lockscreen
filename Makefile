
all: lockscreen

lockscreen: lockscreen.c
	clang -F /System/Library/PrivateFrameworks \
	    -framework login -framework IOKit -framework Foundation \
		-framework ApplicationServices \
	    -mmacosx-version-min=10.6 \
	    -o lockscreen lockscreen.c

install:
	mkdir -p ~/bin
	install -m 0750 lockscreen ~/bin/lockscreen
	cp -a Lock\ Screen\ and\ Sleep.workflow ~/Library/Services/Lock\ Screen\ and\ Sleep.workflow

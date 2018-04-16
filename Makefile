
all: lockscreen

lockscreen: lockscreen.c
	clang -F /System/Library/PrivateFrameworks \
	    -framework login -framework IOKit -framework Foundation \
		-framework ApplicationServices \
	    -mmacosx-version-min=10.6 \
	    -o lockscreen lockscreen.c

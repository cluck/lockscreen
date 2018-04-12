To compile

    make

or 

	clang -F /System/Library/PrivateFrameworks \
		-framework login -framework IOKit -framework Foundation \
		-mmacosx-version-min=10.6 \
		-o lockscreen lockscreen.c

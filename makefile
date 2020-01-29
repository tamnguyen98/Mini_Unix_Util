all :
	gcc -o my-uniq my-uniq.c -Wall -Werror
	gcc -o my-sed my-sed.c -Wall -Werror
	gcc -o my-cat my-cat.c -Wall -Werror

clean :
	rm my-uniq
	rm my-sed
	rm my-cat

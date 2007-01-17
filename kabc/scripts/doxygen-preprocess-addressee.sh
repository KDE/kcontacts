if test "$1" = "create"; then
	./makeaddressee
elif test "$1" = "cleanup"; then
	rm -f ../addressee.h ../addressee.cpp ../field.cpp
fi

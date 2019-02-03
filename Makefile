all: main 
main: prover.o main.cpp boolexp.o varexp.o notexp.o andexp.o orexp.o condexp.o universalexp.o existentialexp.o subject.o
	g++ -g -Wall -pedantic main.cpp varexp.o boolexp.o notexp.o andexp.o orexp.o condexp.o universalexp.o existentialexp.o subject.o prover.o -o main
#main: main.cpp boolexp.o varexp.o notexp.o andexp.o orexp.o condexp.o universalexp.o existentialexp.o subject.o
	#g++ -g -Wall -pedantic main.cpp varexp.o boolexp.o notexp.o andexp.o orexp.o condexp.o universalexp.o existentialexp.o subject.o -o main

subject.o: subject.h subject.cpp
	g++ -g -Wall -pedantic subject.cpp -o subject.o -c
boolexp.o: boolexp.h boolexp.cpp
	g++ -g -Wall -pedantic boolexp.cpp -o boolexp.o -c
varexp.o: varexp.h varexp.cpp
	g++ -g -Wall -pedantic varexp.cpp -o varexp.o -c
notexp.o: notexp.h notexp.cpp
	g++ -g -Wall -pedantic notexp.cpp -o notexp.o -c
andexp.o: andexp.h andexp.cpp
	g++ -g -Wall -pedantic andexp.cpp -o andexp.o -c
orexp.o: orexp.h orexp.cpp
	g++ -g -Wall -pedantic orexp.cpp -o orexp.o -c
condexp.o: condexp.h condexp.cpp
	g++ -g -Wall -pedantic condexp.cpp -o condexp.o -c
universalexp.o: universalexp.h universalexp.cpp
	g++ -g -Wall -pedantic universalexp.cpp -o universalexp.o -c
existentialexp.o: existentialexp.h existentialexp.cpp
	g++ -g -Wall -pedantic existentialexp.cpp -o existentialexp.o -c
prover.o: prover.h prover.cpp
	g++ -g -Wall -pedantic prover.cpp -o prover.o -c

clean:
	./clean.sh
DATE:=$(shell date +"%d-%a-%b-%Y_%H_%M_%S")
backup:
	tar -cvf superExprTest-$(DATE)-backup.tar .
	gzip -9 superExprTest-$(DATE)-backup.tar
	mv superExprTest-$(DATE)-backup.tar.gz OldCodeVersions

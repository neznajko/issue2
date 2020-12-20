task2 = Shopoff
flags = -Xlint -g
java: ${task2}.java
	javac ${flags} $^ && java ${task2}
.PHONY: clean
clean:
	${RM} -v ${wildcard *.class}

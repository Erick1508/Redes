JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
        Comandos.java \
        ComandosImpl.java \
        Validar_Interface.java \
        Validar_InterfaceImpl.java \
        a_rmifs.java \
        s_rmifs.java \
        c_rmifs.java 

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class *~
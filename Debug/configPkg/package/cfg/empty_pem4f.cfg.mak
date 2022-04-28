# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f E:\Users\Azman\workspace_v6_2\ms8/src/makefile.libs

clean::
	$(MAKE) -f E:\Users\Azman\workspace_v6_2\ms8/src/makefile.libs clean


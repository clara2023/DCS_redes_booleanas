# 


## Boolean Network
Este projeto corresponde ao Desenvolvimento de Componentes de Simulação (DCS), como descrito na disciplina INE5425 (Modelagem e Simulação). O projeto integra um componente de simulação denominado BooleanNetwork ao simulador Genesys e implementa o arquivo de simulação Smart_BooleanNetwork.

## Compilando o Smart_BooleanNetwork
Na raíz do projeto
```console
make -C projects/GenesysTerminalApplication
```

## Executando
Na raíz do projeto
```console
 ./projects/GenesysTerminalApplication/dist/genesysterminalapplication 
```

## Estrutura
Navegue, a partir da raíz do projeto, para o caminho: 
```console
source/applications/terminal/examples/smarts
```
E adicione os arquivos Smart_BooleanNetwork.cpp e Smart_BooleanNetwork.h.

Adicione os componentes BooleanNetwork.cpp e BooleanNetwork.h no caminho
```console
source/plugins/components
```

Na raíz do projeto, acrescente o novo componente no arquivo autoloadplugins.txt
```console
booleannetwork.so
```

Para cada Smart criado, altere os arquivos Makefile-Release e Makefile-Debug para incluir o elemento Smart criado.

Acrescente o componente na sessão OBJECTFILES.
```console
	${OBJECTDIR}/_ext/f13e5db9/BooleanNetwork.o \
```

```console
# Build Targets

.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/genesysterminalapplication

${CND_DISTDIR}/genesysterminalapplication: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}
	${LINK.cc} -o ${CND_DISTDIR}/genesysterminalapplication ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/6bf258f7/BaseGenesysTerminalApplication.o: ../../source/applications/BaseGenesysTerminalApplication.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/6bf258f7
	$(COMPILE.cc) -g -I../../source/gtest -std=c++14 -o ${OBJECTDIR}/_ext/6bf258f7/BaseGenesysTerminalApplication.o ../../source/applications/BaseGenesysTerminalApplication.cpp
```

```console
# Compilar Smart

${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork.o: ../../source/applications/terminal/examples/smarts/Smart_BooleanNetwork.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext
	$(COMPILE.cc) -g -I../../source/gtest -std=c++14 -o ${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork.o ../../source/applications/terminal/examples/smarts/Smart_BooleanNetwork.cpp

${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork_nomain.o: ${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork.o ../../source/applications/terminal/examples/smarts/Smart_BooleanNetwork.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/296208d5
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    $(COMPILE.cc) -g -I../../source/gtest -std=c++14 -Dmain=__nomain -o ${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork_nomain.o ../../source/applications/terminal/examples/smarts/Smart_BooleanNetwork.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork.o ${OBJECTDIR}/_ext/296208d5/Smart_BooleanNetwork_nomain.o;\
	fi
```

Acrescentar o componente no arquivo source/kernel/simulator/PluginConnectorDummyImpl1.cpp

```console
	else if (fn == "booleannetwork.so")
		GetInfo = &BooleanNetwork::GetPluginInformation;
```




















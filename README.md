# 



# Passo-a-passo de como criar seu próprio projeto
## Compilação
Na raíz do projeto
```console
make -C projects/GenesysTerminalApplication
```
Esse comando criará um arquivo binário em `projects/GenesysTerminalApplication/dist`. Para executar, esse arquivo precisa ter na mesma pasta o arquivo `autoloadplugins.txt`

Inicialmente, o repositório está escrito para criar uma aplicação que demonstra o funcionamento de um componente chamado `SeizeDelayRelease` pela aplicação `Smart_SeizeDelayRelease`

## Compilando o *seu* projeto
A definicação de qual aplicação será compilada está no arquivo `source/applications/terminal/TraitsTerminalApp.h`, nas linhas 17 e 58:
```cpp
#include "examples/smarts/Smart_SeizeDelayRelease.h"
.
.
.
typedef Smart_SeizeDelayRelease Application;
```

Para fazer a sua própria aplicação, basta substituir a declaração do Smart_SeizeDelayRelease pelo smart criado pelo seu grupo.

A classe `Smart_SeizeDelayRelease` está definida em `source/applications/terminal/examples/smarts/Smart_SeizeDelayRelease.cpp`. Você pode copiar esse arquivo para definir a sua própria aplicação, que, por sua vez, utilizará os componentes desenvolvidos pelo grupo para entrega do trabalho.

## Criação dos componentes
No GenESyS, "componentes" referem-se a plugins, que definirão o comportamento de cada bloco que o usuário pode usar na interface gráfica ou, como se deve fazer para o trabalho, em código que utilize o GenESyS como biblioteca.

Nos arquivos de aplicações (por exemplo o `source/applications/terminal/examples/smarts/Smart_SeizeDelayRelease.cpp`), pode-se notar a inclusão de diversos plugins das linhas 22 a 26:
```cpp
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Seize.h"
#include "../../../../plugins/components/Delay.h"
#include "../../../../plugins/components/Release.h"
#include "../../../../plugins/components/Dispose.h"
```
Além disso, a modelagem de um sistema que utiliza esses componentes pode ser encontrada na implementação da classe Smart_SeizeDelayRelease nesse mesmo arquivo.

Dessa forma, basta:
- Criar o seu código fonte na pasta `source/plugins/components` definindo o comportamento dos componentes necessários para o seu respectivo trabalho.
- Incluí-los na definição da sua aplicação em `source/applications/terminal/examples/smarts/`. A sua aplicação **poderia** ser algo diferente de um smart, mas eu não sei o suficiente sobre isso pra descrever como seria qualquer outra possibilidade ¯\\\_(ツ)\_/¯
- Alterar `source/applications/terminal/TraitsTerminalApp.h` para utilizar o seu Smart em vez do Smart_SeizeDelayRelease
- Compilar o projeto com `make -C projects/GenesysTerminalApplication`
- Incluir o seu plugin na lista de plugins necessários em `/autoloadplugins.txt`
- Executar com
```bash
cd projects/GenesysTerminalApplication/dist && ./genesysterminalapplication
```


# Exemplo de plugin Trabalho usado no Smart_Trabalho
Atualmente, o projeto GenesysTerminalApplication está construído de forma a executar o Smart definido em `source/applications/terminal/examples/smarts/Smart_Trabalho.cpp`.

A construção desse arquivo foi feita baseada no passo-a-passo anterior. Agora, segue a lista de ações necessária para adicionar esse novo arquivo no processo de compilação do projeto.

Para esse passo, por favor acompanhe as mudanças aplicadas no commit "6042927b4: Criar um Componente customizado "Trabalho" e um Smart que o utiliza para servir de exemplo para futuros trabalhos" e replique-as para cada arquivo criado pelo seu grupo.

<!-- TODO -->
**AVISO:** O projeto possui 2 makefiles, um para debug e outro para release. Eu entendo que o makefile para release deve chamar o compilador com flags de otimização para release do projeto. Porém, no momento, eu só queria que essa bomba compilasse, então tanto o make debug quanto release compilam os arquivos criados da mesma forma.

Daqui em diante, irei me referir ao `Makefile-Debug.mk` como makefile, e deixarei de lado o `Makefile-Release`.

## Smart criado
Adicione uma linha referenciando o seu smart criado na lista de arquivos objetos a serem criados pelo compilador.

No trecho de código a seguir, `Smart_WaitSignal.o` é um smart já presente no arquivo, pelo qual você pode se orientar para criar a linha do `Smart_Trabalho.o` (Ou o nome que você der para o arquivo do seu smart)
```make
${OBJECTDIR}/_ext/296208d5/Smart_WaitSignal.o \
${OBJECTDIR}/_ext/296208d5/Smart_Trabalho.o \
```

## Plugin
O mesmo precisa ser feito para os arquivos dos plugins definidos pelo enunciado do trabalho.

No caso desse exemplo, apenas um plugin foi criado (`source/plugins/components/Trabalho.cpp`), então apenas uma linha é necessária. Porém esse processo precisa ser replicado para cada arquivo de implementação criado pelo grupo.

De forma análoga ao exemplo anterior, no código seguinte, `Write.o` é um plugin já existe no código fonte do GenESyS e `Trabalho.o` é o plugin criado para o exemplo. **Note que** a pasta `f13e5db9` dos plugins é diferente da pasta `296208d5` dos smarts.

```make
${OBJECTDIR}/_ext/f13e5db9/Write.o \
${OBJECTDIR}/_ext/f13e5db9/Trabalho.o \
```

## Regras de compilação
Uma vez definidos os códigos objetos a serem gerados para o projeto, é preciso definir como cada um desses arquivos será compilado. O seguinte conjunto de linhas do makefile serve de exemplo:

`projects/GenesysTerminalApplication/nbproject/Makefile-Debug.mk`
```make
# Compilar Smart
${OBJECTDIR}/_ext/296208d5/Smart_Trabalho.o: ../../source/applications/terminal/examples/smarts/Smart_Trabalho.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext
	$(COMPILE.cc) -g -I../../source/gtest -std=c++14 -o ${OBJECTDIR}/_ext/296208d5/Smart_Trabalho.o ../../source/applications/terminal/examples/smarts/Smart_Trabalho.cpp

${OBJECTDIR}/_ext/296208d5/Smart_Trabalho_nomain.o: ${OBJECTDIR}/_ext/296208d5/Smart_Trabalho.o ../../source/applications/terminal/examples/smarts/Smart_Trabalho.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/296208d5
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/296208d5/Smart_Trabalho.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    $(COMPILE.cc) -g -I../../source/gtest -std=c++14 -Dmain=__nomain -o ${OBJECTDIR}/_ext/296208d5/Smart_Trabalho_nomain.o ../../source/applications/terminal/examples/smarts/Smart_Trabalho.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/296208d5/Smart_Trabalho.o ${OBJECTDIR}/_ext/296208d5/Smart_Trabalho_nomain.o;\
	fi
```

Note que no código acima, estão definidas apenas as regras para compilação do smart. No diff do commit 6042927b4, você vai encontrar também as regras para compilação do plugin `Trabalho.cpp`.

No makefile original, existe uma certa organização dessas regras, porém, para que o commit ficasse mais conciso e fácil de identificar tudo que precisa ser feito, eu coloquei tudo junto mesmo.

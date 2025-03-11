# Configuração do Ambiente para Computação Gráfica com <img src="https://github.com/user-attachments/assets/db96047c-5e1a-4656-a3af-ee3432f9f35a" alt="OpenGL Logo" height="32" style="vertical-align: middle;" />

Este tutorial irá guiá-lo pela instalação e configuração do ambiente para rodar projetos OpenGL com **CMake** e **VS Code**, utilizando o **MinGW-UCRT64** como compilador (portanto, no **Windows** <img src="https://github.com/user-attachments/assets/ad835449-5f61-4907-a116-b9ced2c8e30d" alt="MS Windows Logo" height="16" style="vertical-align: middle;" />).

## ⚠️ Importante: Diferenças entre Windows, Linux e macOS

Este tutorial foi desenvolvido para Windows utilizando MSYS2 UCRT64 como ambiente de compilação. Caso esteja utilizando Linux ou macOS, algumas configurações podem ser diferentes, especialmente na escolha do compilador C/C++ e na configuração do CMake.

Para configurar corretamente o compilador no VS Code no Linux ou no MacOS, siga os guias oficiais:

🔗 [Linux: Configuração do VS Code para C++ no Linux](https://code.visualstudio.com/docs/cpp/config-linux)  
🔗 [macOS: Configuração do VS Code para C++ no macOS](https://code.visualstudio.com/docs/cpp/config-clang-mac)  

Caso tenha dificuldades na configuração do CMake, consulte a documentação oficial:  
🔗 [CMake Documentation](https://cmake.org/documentation/)

---

## 📌 1. Instalando as Ferramentas Necessárias

Antes de começar, certifique-se de ter os seguintes programas instalados:

### 1️⃣ Instalar o <img src="https://github.com/user-attachments/assets/5e9c8078-53b0-4497-bbf2-1f379293aa60" alt="CMake Logo" height="32" style="vertical-align: middle;" />

Baixe e instale o **CMake** a partir do site oficial:
🔗 [CMake Download](https://cmake.org/download/)

Durante a instalação, **habilite a opção "Add CMake to system PATH"** para facilitar o uso no terminal.

---

### 2️⃣ Instalar o Compilador MinGW-UCRT64 através do <img src="https://github.com/user-attachments/assets/a473c44b-1fe1-4399-911a-d921225f53a6" alt="MSYS2 Logo" width="32" style="vertical-align: middle;" />


Baixe o **MSYS2** através do link:
🔗 [MSYS2 Download](https://www.msys2.org/)

Provavelmente ao terminar de instalar, abrirá um terminal. Caso isso não aconteça, digite "msys" na barra de pesquisa do Windows e entre neste aplicativo:

 <img src="https://github.com/user-attachments/assets/8d5603b3-6338-4235-9775-6aeaa900da7d" alt="" width="256" align="middle"/>

Execute o seguinte comando para instalar os pacotes necessários:

```sh
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

Caso queira ou necessite de mais suporte nesta etapa, consulte o manual oficial:

🔗 [Configuração do VS Code para C++ no Windows](https://code.visualstudio.com/docs/cpp/config-mingw)

### Configurando a variável de ambiente no Sistema Operacional (Windows)

Esse passo garante que o sistema operacional encontre o compilador automaticamente ao rodar comandos no terminal, sem precisar especificar o caminho completo. A forma mais simples de fazer isso (se você tiver permissão de administrador do sistema) é a seguinte:

 - Edite a variável de caminhos do sistema operacional (`PATH`), acrescentando o caminho onde ficaram os executáveis do compilador (provavelmente ficaram em `C:\msys64\ucrt64\bin` - ou onde você escolheu instalar)

<img src="https://github.com/user-attachments/assets/645846a5-38e2-4a26-a307-a961afb80116" alt="" style="width:30%; height:auto;" align="middle"/>
<img src="https://github.com/user-attachments/assets/89bc66d1-c269-45ac-9f5f-e528325a9e95" alt="" style="width:30%; height:auto;" align="middle"/>
<img src="https://github.com/user-attachments/assets/25c305fa-239c-4119-afd3-20d8dbf318e6" alt="" style="width:30%; height:auto;" align="middle"/>

 - Se estiver em um computador que não tenha acesso de adm, adicionar temporariamente ao path com este comando (via terminal CMD): 
```sh
   set PATH=%PATH%;C:\msys64\ucrt64\bin
```
---

### 3️⃣ Instalar o VS Code <img src="https://github.com/user-attachments/assets/0b0e314b-8910-4311-b862-8f4c2e012d33" alt="Visual Studio Code Logo" width="32" style="vertical-align: middle;" />

Baixe e instale o **VS Code** pelo link:
🔗 [VS Code Download](https://code.visualstudio.com/)

Após a instalação, abra o **VS Code** e instale as seguintes extensões:

- **CMake Tools** ➝ Para integração com o CMake.
- **C/C++** ➝ Para suporte à IntelliSense e depuração.
  
Para isso, você pode ir no menu View -> Extensions ou clicar no ícone <img src="https://github.com/user-attachments/assets/ce6873db-1dbb-4a74-887f-4ea90f90dfc4" alt="" width="32" style="vertical-align: middle;" /> da interface do Visual Studio Code.

---

## 📌 2. Clonando o Repositório de Exemplo

Agora vamos baixar o código de exemplo:

1️⃣ **Clone o repositório** no diretório de sua escolha:

```sh
git clone https://github.com/fellowsheep/CGCCHIB.git
```
Se você nunca usou o git, pode fazê-lo utilizando o 🔗 [Github Desktop](https://desktop.github.com/download/). Para isso, consulte o seguinte material:
- [Tutorial de Entregas pelo Github](TutorialEntregasGithub.pdf)

2️⃣ Abra o VS Code e vá em `Arquivo -> Abrir Diretório` (ou `File -> Open Folder`). Selecione o diretório onde você clonou o repositório de exemplo.

<img src="https://github.com/user-attachments/assets/501ad8f1-30cc-44b0-bec0-8ef650a79998" alt="" style="width:30%; height:auto;" align="middle"/>

⚠️ É muito importante entrar no diretório do projeto pelo comando `Open Folder`. Caso contrário, o VS Code não reconhece o diretório `${workspaceFolder}`, o que poderá causar problemas no passo de compilação.

---

## 📌 3. Configurando o CMake no VS Code

1️⃣ No VS Code, pressione `Ctrl + Shift + P` e procure por `CMake: Scan for kit`

<img src="https://github.com/user-attachments/assets/93417f3a-6fc3-448c-bb73-1cc3a33ab199" alt="" style="width:40%; height:auto;" align="middle"/>

2️⃣ Logo após, pressione novamente `Ctrl + Shift + P` e procure por `CMake: Select a Kit`.  

3️⃣ **Selecione o compilador correto**:
   - Escolha `GCC for MSYS2 UCRT64` (ou similar, conforme sua instalação)
     
     <img src="https://github.com/user-attachments/assets/618655d3-253a-4d99-a6c0-e241c45a5c0c" alt="" style="width:40%; height:auto;" align="middle"/>

4️⃣ Este passo costuma acontecer automaticamente. Mas caso não ocorra nada após selecionar o kit, pressione `Ctrl + Shift + P` e execute `CMake: Configure`
   - Isso fará o **CMake detectar o compilador correto e preparar o projeto**. Espera-se que, após esta etapa, tenha-se uma saída similar a esta no terminal do VS Studio Code:
  
   <img src="https://github.com/user-attachments/assets/ca3bc7f1-5115-48d2-9f39-6a236e18f9b3" alt="" style="width:50%; height:auto;" align="middle"/>

## 📌 4. Compilando e Executando o Projeto

Da mesma forma que o **CMake: Configure** pode executar automaticamente, a compilação pode ocorrer em sequência também de forma automática.
Caso não ocorra ou você pretenda compilar novamente:

1️⃣ Pressione `Ctrl + Shift + P` e execute `CMake: Build`
   - Ou rode manualmente no terminal:

   ```sh
   cd build
   cmake --build .
   ```

2️⃣ **Execute o programa**:
   ```sh
   ./Hello3D.exe
   ```

Se tudo estiver correto, o projeto será compilado e executado com sucesso! 🚀

---

## 📌 5. Próximos Passos

Agora que seu ambiente está configurado, você pode:

✅ Copiar o conteúdo do repositório clonado para o **seu repositório**. Caso precise de ajuda para criar e organizar seu próprio repositório de exercícios, consulte os seguintes materiais:
- [Tutorial de Entregas pelo Github](TutorialEntregasGithub.pdf)
- [Organizando seu repositório no Github](OrganizandoRepositorioGithub.pdf)

✅ Adicionar os exercícios dentro da pasta `src/`
```
📁 src/
│── Hello3D.cpp
│── Transformacoes.cpp
│── Iluminacao.cpp
```

✅ Para cada exercício novo, atualizar o arquivo `CMakelists.txt`
```cmake
set(EXERCISES 
    Hello3D
    Transformacoes
    Iluminacao
)
```
✅ Atualmente, o `CMakelists.txt` já está configurado para compilar e gerar o excutável de cada código acrescentado no set EXERCISES. Se necessário, adicionar novas dependências
```cmake
foreach(EXERCISE ${EXERCISES})
    add_executable(${EXERCISE} src/${EXERCISE}.cpp Common/glad.c)
    target_link_libraries(${EXERCISE} glfw opengl32)
endforeach()
```
✅ Isso faz com que cada exercício gere seu próprio executável dentro da pasta build/.

✅ Portanto, se adicionar mais arquivos .cpp, basta incluir o nome na lista EXERCISES e rodar o CMake novamente.

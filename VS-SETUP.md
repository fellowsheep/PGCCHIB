# Configurando projetos em C++ com OpenGL no Visual Studio 
## Um mini-tutorial

Esse tutorial funciona para as versões mais atuais do Visual Studio (2019, 2022, ...)

Uma coisa muito importante, ao configurar um projeto no VS, é escolher para qual arquitetura que o projeto será configurado. Para isso, certifique-se se suas dependências que possuem bibliotecas estáticas (.lib) e/ou dinâmicas (.dll) estão compiladas em x86 ou x64 (arquiteturas 32 ou 64 bits). Considerando as dependências já existentes neste repositório, usaremos a arquitetura 64 bits. que é a padrão atual:

![image](https://github.com/user-attachments/assets/4e8a62f9-9cce-496c-b050-538c8d4d39df)

Se você quiser ou precisar alterar as dependências, você deve ir em Projeto -> Propriedades e alterar estes 3 lugares:

1) Diretório com os arquivos de cabeçalho (onde ficam os .h da biblioteca): 
![image](https://github.com/user-attachments/assets/14b699ca-90f9-4e7e-9471-238627878011)
No caso do projeto HelloTriangle, que precisa da GLFW e da GLAD, colocaríamos os diretórios (usando caminho relativo):
* ..\Dependencies\glfw-3.4.bin.WIN64\include
* ..\Dependencies\GLAD\include

3) Diretório com os arquivos de biblioteca estática pré-compilados (.lib ou .a):
![image](https://github.com/user-attachments/assets/1518cb32-2d2a-4692-833b-c991ea9a6d90)
No projeto HelloTriangle, apenas a GLFW possui biblioteca estática, a `glfw3.lib`. Ao baixar ela para Windows 64 bits, haverá um diretório `lib-vc2022`. entre outras opções (pode inclusive apagar os diretórios para plataformas que você não pretende usar). Use o caminho relativo para este diretório:
* ..\Dependencies\glfw-3.4.bin.WIN64\lib-vc2022

5) Incluir os nomes das bibliotecas estáticas, em `Vinculador > Entrada > Dependências Adicionais` (não apague as já existentes!):
![image](https://github.com/user-attachments/assets/0693e37d-0209-4fbe-bd91-f5a200a128db)
Neste caso, temos apenas a `glfw3.lib` (precisa escrever). Se houver mais de uma, escreva uma por linha nesta janela que abre ao clicar Editar:

![image](https://github.com/user-attachments/assets/a6b85b55-cd85-40d5-b49c-1c7b0d2596b4)

Ao rodar o programa HelloTriangle, você deverá obter este resultado:
![image](https://github.com/user-attachments/assets/2026a613-1382-4348-bf1a-93b9e24fc152)



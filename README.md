
# APS — Sistema de Busca e Ordenação (Pará 2023/2024)

## Descrição do Projeto

Este projeto implementa um **Sistema de Busca e Ordenação de Dados** referentes aos focos de queimadas no estado do **Pará**, utilizando dados dos anos **2023** e **2024**.

O programa:
- Lê arquivos CSV (`focos_br_pa_ref_2023.csv` e `focos_br_pa_ref_2024.csv`);
- Permite ordenação por **município**, **bioma** ou **data/hora**;
- Implementa **8 algoritmos de ordenação**:
  1. Bubble Sort  
  2. Selection Sort  
  3. Insertion Sort  
  4. Shell Sort  
  5. Merge Sort  
  6. Quick Sort  
  7. Heap Sort  
  8. Bogo Sort  
- Mede o número de **comparações**, **trocas** e o **tempo total de execução**;
- Exibe resultados **200 registros por página**, com opção de continuar.

---

## Estrutura dos Arquivos

Certifique-se de que todos os arquivos estejam na **mesma pasta**:

```

📁 APS2025.2/
├── main.c
├── focos_br_pa_ref_2023.csv
├── focos_br_pa_ref_2024.csv
└── README.md

```

---

## Instalação do Compilador (MinGW)

### 1. Baixar o MinGW-w64

Acesse o site oficial:  
👉 [https://sourceforge.net/projects/mingw-w64/](https://sourceforge.net/projects/mingw-w64/)

Clique em **Download (mingw-w64)**.  
O instalador baixado será algo como `mingw-w64-install.exe`.

---

### 2. Instalar o MinGW-w64

Durante a instalação, selecione as opções:

| Campo | Valor recomendado |
|-------|-------------------|
| **Version** | latest |
| **Architecture** | x86_64 |
| **Threads** | posix |
| **Exception** | seh |
| **Build revision** | latest |

Escolha o diretório de instalação:
```

C:\mingw-w64

```

Finalize clicando em **Finish**.

---

### 3. Adicionar o MinGW ao PATH

1. Pressione `Win + R` → digite `sysdm.cpl`  
2. Vá em **Avançado → Variáveis de ambiente**  
3. Em **Variáveis do sistema**, localize `Path` → clique em **Editar**  
4. Clique em **Novo** e adicione:
```

C:\mingw-w64\bin

````
5. Confirme com **OK** em todas as janelas.

---

> *Link youtube, vídeo de suporte -> https://youtu.be/3pfRvy_gfqY?si=PsiDk5G7IFfIEnvR*

---

### 4. Testar a instalação

Abra o **Prompt de Comando** e digite:

```bash
gcc --version
````

Se aparecer algo como abaixo, está tudo certo:

```
gcc (x86_64-posix-seh, built by ... ) <versão>
```

---

## Configurando o VS Code

### Instalar o Visual Studio Code

Baixe e instale em:
👉 [https://code.visualstudio.com/](https://code.visualstudio.com/)

---

### Instalar Extensões Necessárias

No VS Code, instale as seguintes extensões:

* `C/C++` — (Microsoft) — fornece IntelliSense e debugging
* `Code Runner` — (opcional) para rodar o código facilmente
* `C/C++ Compile Run` — alternativa ao Code Runner

---

### Verificar o Compilador no VS Code

Abra o terminal (`Ctrl + '` ou **Exibir → Terminal**) e digite:

```bash
gcc --version
```

Se aparecer a versão, o compilador está configurado corretamente.

---

## Como Compilar e Executar o Programa

### Opção 1 — Terminal do VS Code

1. Navegue até a pasta do projeto:

   ```bash
   cd "C:\Users\SeuUsuario\Desktop\APS2025.2"
   ```

2. Compile o programa:

   ```bash
   gcc main.c -o aps -lm
   ```

3. Execute:

   ```bash
   ./aps
   ```

---

### Opção 2 — Usando o Code Runner

1. Abra o arquivo `main.c`
2. Clique com o **botão direito** → **Run Code**
3. O VS Code compilará e executará automaticamente no terminal integrado

---

## Funcionamento do Programa

1. Escolha o **ano**:

   ```
   1 - Ano 2023
   2 - Ano 2024
   0 - Sair
   ```

2. Escolha o **campo de ordenação**:

   ```
   1 - Município
   2 - Bioma
   3 - Data
   ```

3. Escolha o **algoritmo** (1 a 8)

4. O programa exibirá:

   * Número total de registros
   * Quantidade de comparações e trocas
   * Tempo total de execução

5. Exibe **200 registros por vez**, perguntando:

   ```
   Deseja ver mais 200 registros? (S/N)
   ```

---

## Exemplo de Execução

```text
APS - Estrutura de Dados (Pará 2023/2024)
Certifique-se de que os arquivos CSV estão na mesma pasta do programa.

=== APS - Ordenação de Dados (Pará) ===
1 - Ano 2023
2 - Ano 2024
0 - Sair
Escolha: 1

Carregando arquivo focos_br_pa_ref_2023.csv ...
Registros carregados: 15000

Ordenar por:
1 - Município
2 - Bioma
3 - Data
Escolha: 1

Escolha o algoritmo:
1 - Bubble Sort
2 - Selection Sort
3 - Insertion Sort
4 - Shell Sort
5 - Merge Sort
6 - Quick Sort
7 - Heap Sort
8 - Bogo Sort
Escolha: 6

--- Estatísticas ---
Registros: 15000
Comparações: 176000
Trocas: 90500
Tempo total: 0.684122 s

--- Mostrados 200/15000 registros ---
Comparações: 176000 | Trocas: 90500 | Tempo acumulado: 0.684122 s
Deseja ver mais 200 registros? (S/N):
```

---

## Requisitos Técnicos

| Requisito              | Detalhe                         |
| ---------------------- | ------------------------------- |
| Sistema Operacional | Windows 10 ou 11                |
| Compilador          | MinGW-w64 (GCC 9.2 ou superior) |
| Padrão C            | ANSI C (C89/C90)                |
| Editor              | Visual Studio Code              |
| Memória             | ≥ 4 GB RAM                      |

---

## Créditos

**Disciplina:** Estrutura de Dados
**Projeto:** APS — Sistema de Ordenação de Focos (Pará 2023/2024)

---

> *Certifique-se de manter os arquivos CSV na mesma pasta do programa antes de executar.*

```



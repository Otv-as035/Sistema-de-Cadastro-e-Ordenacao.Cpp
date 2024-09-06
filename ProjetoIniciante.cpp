/* 	*****Projeto: Times da NBA *******
 
 * nome: Otávio Augusto Silva.
 * nome: karen Cristina Inácio.
 * nome: Taylor willian da Silva Monteiro.
 
 * */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <locale> 

using namespace std;

struct TimesDaNBA {
    int identificador;
    string nomeDoTime;
    string nomeDoJogador;
    int numeroDeTitulos;
    float mediaDePontos;
};

// Funções auxiliares
bool leitor(TimesDaNBA *&times, int &tamanhoDoVetor, int &capacidade);
void salvarTimesNoArquivo(TimesDaNBA *times, int capacidade);
void menu();
void inserirElementoOrdenado(TimesDaNBA *&times, int &tamanhoDoVetor, int &capacidade);
void exibirTimes(TimesDaNBA *times, int capacidade);
void exibirTimesAtivos(TimesDaNBA *times, int capacidade);
void redimensionarVetor(TimesDaNBA *&times, int &tamanhoDoVetor);
void removerElemento(TimesDaNBA *&times, int &capacidade);
bool perguntarSalvarAlteracoes();
void buscarPorNome(TimesDaNBA *times, int &capacidade);
void ordenarPorNome(TimesDaNBA *times, int capacidade);
void ordenarPorMediaDePontos(TimesDaNBA *times, int capacidade);
int buscaBinariaNome(TimesDaNBA *times, int inicio, int fim, string &nome);
void quickSort(TimesDaNBA *times, int inicio, int fim, bool ordenarPorNome);
int particionar(TimesDaNBA *times, int inicio, int fim, bool ordenarPorNome);
void ordenarPorMediaDePontos2(TimesDaNBA *times, int capacidade);
int buscaBinariaMediaDePontos(TimesDaNBA *times, int inicio, int fim, float mediaDePontos);
void exibirTrechoArquivo(int inicio, int fim);

int main() {  /****** PRINCIPAL ************/
	
    setlocale(LC_ALL, "portuguese");

    int tamanhoDoVetor = 40;
    int capacidade = 0;
    TimesDaNBA *times = new TimesDaNBA[tamanhoDoVetor];

    bool leitura = leitor(times, tamanhoDoVetor, capacidade);
    
    if (leitura) {
        int operacao;
        menu();
        cin >> operacao;
        
        while (operacao != 6) {
            switch (operacao) {
                case 1:
                    system("clear"); // utilizado para limpar a tela para sistema Linux
                    inserirElementoOrdenado(times, tamanhoDoVetor, capacidade);
                    break;

                case 2:
					system("clear");
                    removerElemento(times, capacidade);
                    break;

                case 3:
                system("clear");
				cout << "Escolha o tipo de busca:" << endl;
				cout << "1 - Buscar por nome do time" << endl;
				cout << "2 - Buscar por media de pontos" << endl;
				int opcaoBusca;
				cin >> opcaoBusca;

				if (opcaoBusca == 1) {
					buscarPorNome(times, capacidade);
				} else if (opcaoBusca == 2) {
					// Ordenar o vetor por média de pontos antes da busca
					ordenarPorMediaDePontos2(times, capacidade);

					float media;
					cout << "Digite a media de pontos que deseja buscar: ";
					cin >> media;

					int index = buscaBinariaMediaDePontos(times, 0, capacidade - 1, media);

					if (index != -1) {
						cout << "Time encontrado:" << endl;
						cout << "ID: " << times[index].identificador << endl;
						cout << "Nome: " << times[index].nomeDoTime << endl;
						cout << "Jogadores: " << times[index].nomeDoJogador << endl;
						cout << "Numero de Titulos: " << times[index].numeroDeTitulos << endl;
						cout << "Media de Pontos: " << times[index].mediaDePontos << endl;
					} else {
						cout << "Media de pontos nao encontrada!" << endl;
					}
				} else {
					cout << "Opcao inválida!" << endl;
				}
                    break;

                case 4:
					system("clear");
                    cout << "\nEscolha uma opcao:" << endl;
                    cout << "1 - Exibir o arquivo inteiro" << endl;
                    cout << "2 - Exibir um trecho do arquivo" << endl;
                    int opcaoExibir;
                    cin >> opcaoExibir;

                    if (opcaoExibir == 1) {
                        exibirTrechoArquivo(1, capacidade);
                    } else if (opcaoExibir == 2) {
                        int inicio, fim;
                        cout << "Digite o inicio do intervalo: ";
                        cin >> inicio;
                        cout << "Digite o fim do intervalo: ";
                        cin >> fim;
                        exibirTrechoArquivo(inicio, fim);
                    } else {
                        cout << "Opcao inválida!" << endl;
                    }
                    break;

                case 5:
					system("clear");
                    cout << "Escolha o campo para ordenar:" << endl;
                    cout << "1 - Nome do time" << endl;
                    cout << "2 - Media de pontos" << endl;
                    int opcaoOrdenacao;
                    cin >> opcaoOrdenacao;
                    if (opcaoOrdenacao == 1) {
                        ordenarPorNome(times, capacidade);
                    } else if (opcaoOrdenacao == 2) {
                        ordenarPorMediaDePontos(times, capacidade);
                    } else {
                        cout << "Opção invalida!" << endl;
                    }
                    break;
                    
                default:
                    cout << "Opção invalida!" << endl;
                    break;
            }

            // Pergunta se deseja salvar as alterações após cada operação
            if (operacao == 1 || operacao == 2 || operacao == 5) {
                if (perguntarSalvarAlteracoes()) {
                    salvarTimesNoArquivo(times, capacidade);
    
                }
           
            }
                char continuar;
                cout << "\nGostaria de continuar com as operacoes (s/n): ";
                cin >> continuar;
                
                if (continuar == 's' || continuar == 'S') {
                    system("clear");
                    menu();
                    cin >> operacao;
                } else {
                    operacao = 6;
                }
            
            
        }
    } else {
        cout << "Ocorreu um problema com a leitura!" << endl;
    }

    cout << "Saindo do programa..." << endl;
    delete[] times;
    return 0;
}





/*A função leitor lê dados de times da NBA de um arquivo CSV, 
 * armazena em um vetor dinâmico e grava em um arquivo binário.
 *  Se a capacidade do vetor for excedida, ele é redimensionado. 
 * Ao final, os arquivos são fechados, retornando true se tudo
 *  ocorrer corretamente.*/
 
bool leitor(TimesDaNBA *&times, int &tamanhoDoVetor, int &capacidade) {
    ifstream arquivoCSV("dados.csv");
    if (!arquivoCSV) {
        cout << "Erro ao abrir arquivo CSV!" << endl;
        return false;
    }
    
    // Abre o arquivo binário para escrita
    ofstream arquivoBinario("timesDaNBA.bin", ios::binary);
    if (!arquivoBinario) {
        cout << "Erro ao abrir arquivo binario!" << endl;
        return false;
    }
    
    char token;
    while (arquivoCSV >> times[capacidade].identificador) {
        arquivoCSV >> token; // Lê o separador (',')
        getline(arquivoCSV, times[capacidade].nomeDoTime, ',');
        arquivoCSV >> token; // Lê o separador ('"')
        getline(arquivoCSV, times[capacidade].nomeDoJogador, '"');
        arquivoCSV >> token; // Lê o separador (',')
        arquivoCSV >> times[capacidade].numeroDeTitulos;
        arquivoCSV >> token; // Lê o separador (',')
        arquivoCSV >> times[capacidade].mediaDePontos;

        // Grava os dados no arquivo binário
        arquivoBinario.write(reinterpret_cast<char*>(&times[capacidade].identificador), sizeof(times[capacidade].identificador));
        int nomeDoTimeSize = times[capacidade].nomeDoTime.size();
        arquivoBinario.write(reinterpret_cast<char*>(&nomeDoTimeSize), sizeof(nomeDoTimeSize));
        arquivoBinario.write(times[capacidade].nomeDoTime.c_str(), nomeDoTimeSize);
        int nomeDoJogadorSize = times[capacidade].nomeDoJogador.size();
        arquivoBinario.write(reinterpret_cast<char*>(&nomeDoJogadorSize), sizeof(nomeDoJogadorSize));
        arquivoBinario.write(times[capacidade].nomeDoJogador.c_str(), nomeDoJogadorSize);
        arquivoBinario.write(reinterpret_cast<char*>(&times[capacidade].numeroDeTitulos), sizeof(times[capacidade].numeroDeTitulos));
        arquivoBinario.write(reinterpret_cast<char*>(&times[capacidade].mediaDePontos), sizeof(times[capacidade].mediaDePontos));

        capacidade++;

        // Redimensiona o vetor se necessário
        if (capacidade >= tamanhoDoVetor) {
            redimensionarVetor(times, tamanhoDoVetor);
        }
    }
    
    arquivoCSV.close(); // Fecha o arquivo CSV
    arquivoBinario.close(); // Fecha o arquivo binário

    return true;
}







/*A função salvarTimesNoArquivo grava os dados dos times em arquivos 
 * CSV e binário, salvando apenas os elementos com identificadores 
 * válidos (maiores ou iguais a 0). Ela também confirma o sucesso 
 * da operação ao final de cada salvamento.*/
 
void salvarTimesNoArquivo(TimesDaNBA *times, int capacidade) {
   // Salvar em CSV
    ofstream arquivoCSV("dados.csv");
    if (!arquivoCSV) {
        cout << "Erro ao abrir o arquivo CSV para salvar!" << endl;
        return;
    }

    for (int i = 0; i < capacidade; i++) {
        if (times[i].identificador >= 0) {  // Ignora elementos marcados como removidos
            arquivoCSV << times[i].identificador << ","
                       << times[i].nomeDoTime << ",\""
                       << times[i].nomeDoJogador << "\","
                       << times[i].numeroDeTitulos << ","
                       << times[i].mediaDePontos << endl;
        }
    }

    arquivoCSV.close();
    cout << "Dados salvos no arquivo CSV com sucesso!" << endl;

    // Salvar em binário
    ofstream arquivoBinario("timesDaNBA.bin", ios::binary | ios::trunc);
    if (!arquivoBinario) {
        cout << "Erro ao abrir o arquivo binario para salvar!" << endl;
        return;
    }

    for (int i = 0; i < capacidade; i++) {
        if (times[i].identificador >= 0) {  // Ignora elementos marcados como removidos
            // Escreve o identificador
            arquivoBinario.write(reinterpret_cast<const char*>(&times[i].identificador), sizeof(times[i].identificador));
            
            // Escreve o tamanho e o conteúdo do nome do time
            int tamanhoNomeDoTime = times[i].nomeDoTime.size();
            arquivoBinario.write(reinterpret_cast<const char*>(&tamanhoNomeDoTime), sizeof(tamanhoNomeDoTime));
            arquivoBinario.write(times[i].nomeDoTime.c_str(), tamanhoNomeDoTime);

            // Escreve o tamanho e o conteúdo do nome do jogador
            int tamanhoNomeDoJogador = times[i].nomeDoJogador.size();
            arquivoBinario.write(reinterpret_cast<const char*>(&tamanhoNomeDoJogador), sizeof(tamanhoNomeDoJogador));
            arquivoBinario.write(times[i].nomeDoJogador.c_str(), tamanhoNomeDoJogador);

            // Escreve o número de títulos e a média de pontos
            arquivoBinario.write(reinterpret_cast<const char*>(&times[i].numeroDeTitulos), sizeof(times[i].numeroDeTitulos));
            arquivoBinario.write(reinterpret_cast<const char*>(&times[i].mediaDePontos), sizeof(times[i].mediaDePontos));
        }
    }

    arquivoBinario.close();
    cout << "Dados salvos no arquivo binario com sucesso!" << endl;
}







/*A função inserirElementoOrdenado insere um novo time na lista 
 * de forma ordenada com base na média de pontos. Se o vetor estiver
 *  cheio, ele é redimensionado antes da inserção. O novo time é
 *  comparado com os já existentes, sendo inserido na posição correta 
 * para manter a ordenação.*/
 
void inserirElementoOrdenado(TimesDaNBA *&times, int &tamanhoDoVetor, int &capacidade) {
    if (capacidade >= tamanhoDoVetor) {
        redimensionarVetor(times, tamanhoDoVetor);
    }

    TimesDaNBA novoTime;
    
    cout << "Insira o identificador: ";
    cin >> novoTime.identificador;
    cin.ignore();

    cout << "Insira o nome do time: ";
    getline(cin, novoTime.nomeDoTime);

    cout << "Insira o nome do(s) jogador(es): ";
    getline(cin, novoTime.nomeDoJogador);

    cout << "Insira o numero de titulos: ";
    cin >> novoTime.numeroDeTitulos;

    cout << "Insira a media de pontos: ";
    cin >> novoTime.mediaDePontos;
    
    int i;
    for (i = capacidade - 1; (i >= 0 && times[i].mediaDePontos > novoTime.mediaDePontos); i--) {
        times[i + 1] = times[i];
    }
    times[i + 1] = novoTime;
    capacidade++;

    cout << "Elemento inserido com sucesso!" << endl;
}





void exibirTimes(TimesDaNBA *times, int capacidade) {
    for (int i = 0; i < capacidade; i++) {
        cout << "ID: " << times[i].identificador << endl;
        cout << "Time: " << times[i].nomeDoTime << endl;
        cout << "Jogadores: " << times[i].nomeDoJogador << endl;
        cout << "Numero de Titulos: " << times[i].numeroDeTitulos << endl;
        cout << "Media de Pontos: " << times[i].mediaDePontos << endl;
        cout << "-----------------------" << endl;
    }
}







void exibirTimesAtivos(TimesDaNBA *times, int capacidade) {
    for (int i = 0; i < capacidade; i++) {
        if (times[i].identificador >= 0) {  // Exibe apenas elementos não removidos
            cout << "ID: " << times[i].identificador << endl;
            cout << "Time: " << times[i].nomeDoTime << endl;
            cout << "Jogadores: " << times[i].nomeDoJogador << endl;
            cout << "Numero de Titulos: " << times[i].numeroDeTitulos << endl;
            cout << "Media de Pontos: " << times[i].mediaDePontos << endl;
            cout << "-----------------------" << endl;
        }
    }
}





void redimensionarVetor(TimesDaNBA *&times, int &tamanhoDoVetor) {
    int novoTamanho = tamanhoDoVetor * 2;
    TimesDaNBA *novoVetor = new TimesDaNBA[novoTamanho];

    for (int i = 0; i < tamanhoDoVetor; i++) {
        novoVetor[i] = times[i];
    }

    delete[] times;
    times = novoVetor;
    tamanhoDoVetor = novoTamanho;
}





/*A função removerElemento busca um time pelo identificador fornecido 
 * e o marca como removido definindo seu ID como -1. 
 * Se o identificador não for encontrado, exibe uma mensagem de erro.*/
 
void removerElemento(TimesDaNBA *&times, int &capacidade) {
    int id;
    cout << "Digite o ID do time a ser removido: ";
    cin >> id;

    for (int i = 0; i < capacidade; i++) {
        if (times[i].identificador == id) {
            times[i].identificador = -1;  // Marca como removido
            cout << "Elemento removido com sucesso!" << endl;
            return;
        }
    }
    cout << "Elemento não encontrado!" << endl;
}





bool perguntarSalvarAlteracoes() {
    char resposta;
    cout << "Deseja salvar as alteracoes? (s/n): ";
    cin >> resposta;
    if(resposta == 's' || resposta == 'S'){
		return true;
		}else{
			return false;
			}
}





void buscarPorNome(TimesDaNBA *times, int &capacidade) {
    string nome;
    cout << "Digite o nome do time para buscar: ";
    cin.ignore();
    getline(cin, nome);

    // Executa a busca binária
    int index = buscaBinariaNome(times, 0, capacidade - 1, nome);

    if (index != -1) {
        // Se encontrado, exibe os detalhes
        cout << "ID: " << times[index].identificador << endl;
        cout << "Nome do Time: " << times[index].nomeDoTime << endl;
        cout << "Nome do Jogador: " << times[index].nomeDoJogador << endl;
        cout << "Numero de Titulos: " << times[index].numeroDeTitulos << endl;
        cout << "Media de Pontos: " << times[index].mediaDePontos << endl;
    } else {
        // Se não encontrado, exibe uma mensagem de erro
        cout << "Time nao encontrado!" << endl;
    }
}





void ordenarPorNome(TimesDaNBA *times, int capacidade) {
    quickSort(times, 0, capacidade - 1, true);
    exibirTimesAtivos(times, capacidade);
}



void ordenarPorMediaDePontos(TimesDaNBA *times, int capacidade) {
    quickSort(times, 0, capacidade - 1, false);
    exibirTimesAtivos(times, capacidade);
}



int buscaBinariaNome(TimesDaNBA *times, int inicio, int fim, string &nome) {
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        if (times[meio].nomeDoTime == nome) {
            return meio;
        } else if (times[meio].nomeDoTime < nome) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}




void quickSort(TimesDaNBA *times, int inicio, int fim, bool ordenarPorNome) {
    // Verifica se a porção do array ainda pode ser ordenada (início < fim)
    if (inicio < fim) {
        // Chama a função 'particionar' para reorganizar o array e encontrar o pivô
        int pivo = particionar(times, inicio, fim, ordenarPorNome);
        // Recursivamente ordena os elementos à esquerda do pivô
        quickSort(times, inicio, pivo - 1, ordenarPorNome);
        // Recursivamente ordena os elementos à direita do pivô
        quickSort(times, pivo + 1, fim, ordenarPorNome);
    }
}





int particionar(TimesDaNBA *times, int inicio, int fim, bool ordenarPorNome) {
    // Define o pivô como o último elemento do array
    TimesDaNBA pivo = times[fim];
    int i = inicio - 1; // Índice do menor elemento

    // Percorre o array da posição 'inicio' até 'fim - 1'
    for (int j = inicio; j < fim; j++) {
        // Define a condição de comparação baseada no parâmetro 'ordenarPorNome'
        bool condicao = ordenarPorNome ? (times[j].nomeDoTime < pivo.nomeDoTime) : (times[j].mediaDePontos < pivo.mediaDePontos);
        if (condicao) {
            i++; // Incrementa o índice do menor elemento
            swap(times[i], times[j]); // Troca os elementos
        }
    }
    // Troca o pivô com o primeiro elemento maior que ele
    swap(times[i + 1], times[fim]);
    return i + 1; // Retorna a posição do pivô
}





void ordenarPorMediaDePontos2(TimesDaNBA *times, int capacidade) {
    // Chama o QuickSort para ordenar o array por média de pontos (false para não ordenar por nome)
    quickSort(times, 0, capacidade - 1, false);
}





int buscaBinariaMediaDePontos(TimesDaNBA *times, int inicio, int fim, float mediaDePontos) {
    // Caso base: se a busca termina sem encontrar o elemento
    if (inicio > fim) {
        return -1; // Elemento não encontrado
    }

    // Calcula o índice do elemento do meio
    int meio = inicio + (fim - inicio) / 2;

    // Se o elemento do meio tem a média de pontos desejada, retorna o índice
    if (times[meio].mediaDePontos == mediaDePontos) {
        return meio; // Elemento encontrado
    } else if (times[meio].mediaDePontos < mediaDePontos) {
        // Se a média de pontos do meio é menor, busca na metade direita
        return buscaBinariaMediaDePontos(times, meio + 1, fim, mediaDePontos);
    } else {
        // Se a média de pontos do meio é maior, busca na metade esquerda
        return buscaBinariaMediaDePontos(times, inicio, meio - 1, mediaDePontos);
    }
}





void exibirTrechoArquivo(int inicio, int fim) {
    ifstream arquivo("dados.csv");
    if (!arquivo) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    string linha;
    int linhaAtual = 0;
    while (getline(arquivo, linha)) {
        linhaAtual++;
        if (linhaAtual >= inicio && linhaAtual <= fim) {
            cout << linha << endl;
        }
    }

    arquivo.close();
}




void menu() {
    cout << "Menu:" << endl;
    cout << "1 - Inserir um novo elemento" << endl;
    cout << "2 - Remover um elemento" << endl;
    cout << "3 - Buscar" << endl;
    cout << "4 - Exibir" << endl;
    cout << "5 - Ordenar" << endl;
    cout << "6 - Sair" << endl;
    cout << "Escolha uma opcao: ";
}

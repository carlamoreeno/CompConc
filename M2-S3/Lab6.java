// Módulo 2 - Laboratório: 6
// Aluna: Carla Moreno Barbosa
// DRE: 117250502
// Objetivo: Projetar e implementar um programa concorrente onde as
//           Implementar um programa concorrente em Java para somar
//           dois vetores, fazendo C = A + B. Considere que o numero
//           de threads é menor ou igual ao numero de elementos dos
//           vetores.

class Vetor {
	int[] vetor;

	public Vetor(int[] vetor) {
		this.vetor = vetor;
	}

	public int size() {
		return this.vetor.length;
	}

	public int get(int index) {
		return this.vetor[index];
	}

	public boolean set(int index, int num) {
		if ( index >= this.vetor.length ) return false;
		this.vetor[index] = num;
		return true;
	}

	public void print() {
		System.out.print("\nVetor C: ");
		for ( int num : this.vetor ) {
			System.out.print(num + " ");
		}
		System.out.println();
	}
}

// Classe que implementa a interface Runnable
class Soma implements Runnable {

	private Vetor A;
	private Vetor B;
	private Vetor C;
	private int inicio;
	private int fim;

	public Soma(Vetor A, Vetor B, Vetor C, int inicio, int fim) {
		this.A = A;
		this.B = B;
		this.C = C;
		this.inicio = inicio;
		this.fim = fim;
	}

	public void run() {
		for (int i = inicio; i < fim; i++) {
			if ( !this.C.set(i, this.A.get(i) + this.B.get(i)) ) {
				System.out.println("-- Erro");
			}
		}
	}
}

class MainSomaVetor {
	private static int TAM_VETOR = 10; // Tamanho dos vetores do programa
	private static int N = 2; // Número de threads do programa

	public static void main(String args[]) {
		Thread[] threads = new Thread[N];

		// Vetor A é inicializado e corresponde ao intervalo 0 a 9
		int[] a = new int[TAM_VETOR];
		for (int i = 0; i < a.length; i++) {
			a[i] = i;
		}
		System.out.print("Vetor A: ");
		for (int i=0; i < TAM_VETOR; i++) {
			System.out.print(a[i] + " ");
		}

		// Vetor B é inicializado e corresponde ao intervalo 1 a 10
		int[] b = new int[TAM_VETOR];
		for (int i = 0; i < b.length; i++) {
			b[i] = i + 1;
		}
		System.out.print("\nVetor B: ");
		for (int i=0; i < TAM_VETOR; i++) {
			System.out.print(b[i] + " ");
		}

		// Vetor C é inicializado com zeros e corresponderá à soma de A + B
		int[] c = new int[TAM_VETOR];
		for (int i = 0; i < c.length; i++) {
			c[i] = 0;
		}

		Vetor A = new Vetor(a);
		Vetor B = new Vetor(b);
		Vetor C = new Vetor(c);

		int tam = TAM_VETOR / N;
		int resto = TAM_VETOR % N;
		int aux = 0;

		// Cria as threads do programa
		for (int i = 0; i < threads.length; i++) {
			int inicio = aux;
			int fim = inicio + tam;

			aux += tam;
			if ( resto > 0 ) {
				fim += 1;
				aux += 1;
				resto -= 1;
			}

			threads[i] = new Thread(new Soma(A, B, C, inicio, fim));
		}

		// Inicia as threads
		for (int i=0; i<threads.length; i++) {
			threads[i].start();
		}

		// Espera pelo termino de todas as threads
		for (int i=0; i<threads.length; i++) {
			try { threads[i].join(); } catch (InterruptedException e) { return; }
		}

		C.print();
	}
}

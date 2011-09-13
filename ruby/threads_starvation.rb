#====================================================
#Nome: Processos.rb
#Autores: Antonio Canhota, Renato Farias, Rennan Maciel
#Versao: 1.0
#Descricao: Programa que cria 5 threads, ocorrendo starvation
#====================================================


contador = 0
arr = []
#5.times = executar 5 vezes
5.times do |i|
#criando um array de threads, pra facilitar o entendimento
#Thread.new = criacao de thread, vale lembrar que essa thread é independente de SO, ou seja, corre o risco de sofrer starvation pois elas serão rodadas uma de cada vez
  arr[i] = Thread.new {
	#rand utilizado para garantir uma execucao um pouco mais aleatorea... 
	#apos criar cada thread, elas levam um tempo aleatoreo para receber o contador e incrementa-lo
    sleep(rand(0)/10.0)
    Thread.current["meucontador"] = contador
    if (contador == 3)
        puts "LOOP INFINITO! APENAS ESSA THREAD EXECUTA! STARVATION NAS OUTRAS :(" 
	#loop infinito abaixo, ou seja, quando o contador chegar em 3, uma thread executara infinitamente
	#fazendo algumas threads nunca nem serem criadas, e outras sofrerem starvation pela metade
	while(true) do
		#nada, infinitamente :D	
	end
    else
	puts "Sou a thread #{contador} e comecei a executar!"
    end
    contador += 1
  }
#Thread.list retorna a lista de threads criadas por esse programa
#Thread.list.each{ |t| puts t.inspect}
end
arr.each {|t| t.join; print "\nEu sou a thread numero: ",t["meucontador"], ", terminei!", "\n" }
puts "\nQuantidade de threads: #{contador}"


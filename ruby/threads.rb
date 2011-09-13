#====================================================
#Nome: Processos.rb
#Autores: Antonio Canhota, Renato Farias, Rennan Maciel
#Versao: 1.0
#Descricao: Programa que cria 5 threads
#====================================================

lista_de_misses = ["Brasil", "China", "Filipinas", "Ucrania", "Angola"]


contador = 0
arr = []
#5.times = executar 5 vezes
5.times do |i|
#criando um array de threads, pra facilitar a manipulacao
#Thread.new = criacao de thread, vale lembrar que essa thread é independente de SO, ou seja, corre o risco de sofrer starvation pois elas serão rodadas uma de cada vez
#exemplo com starvation segue mais tarde.
  arr[i] = Thread.new {
    #rand utilizado para garantir uma execucao um pouco mais aleatorea... 
    #apos criar cada thread, elas levam um tempo aleatoreo para receber o contador e incrementa-lo
    sleep(rand(0)/10.0)
    #thread.current pega a thread corrente, ou seja, essa thread
    Thread.current["meucontador"] = contador
    #puts Thread.current.inspect
    contador += 1
  }
end
#Thread.list retorna a lista de threads criadas por esse programa
#Thread.list.each{ |t| puts t.inspect}

#t.join: realiza join da thread. quando um programa termina, as threads sao 'killed', nao importando os seus estados.
#para garantir que todas as threads acima executaram ate o fim, fazemos um array.each, chamando join em cada uma das threads
#assim, o join espera que uma thread termine a sua execucao antes de finalizar o programa.
arr.each { |t| t.join; print "\nOla! Eu sou a Miss ",lista_de_misses[t["meucontador"]], "!\n" }
puts "\nQuantidade de threads: #{contador}"


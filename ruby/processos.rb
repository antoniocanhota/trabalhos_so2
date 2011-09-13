#======================================================
#Nome: Processos.rb
#Autores: Antonio Canhota, Renato Farias, Rennan Maciel
#Versao: 1.0
#Descricao: Programa que cria 5 processos
#======================================================



lista_de_comandos = ["gedit threads.rb", "gcalctool", "gnome-terminal", "/usr/games/gnome-sudoku", "/usr/games/gnomine"]

lista_de_comandos.each do |comando|
  exec(comando) unless fork
end

   # para cada elemento do array 'lista_de_comandos', um processo filho sera executado, que rodara um dos comandos acima
   # lista_de_comandos.each do |comando|
   #    if fork == nil
   #       exec(comando)
   #    end
   # processamento continua no processo pai


   # Process.wait espera o processo filho terminar
   # a = Process.wait


# Gedit agora vai executar num processo
# processamento continua no processo pai


# Process.wait espera o processo filho terminar
#a = Process.wait
#b = Process.wait
#puts a
#puts b


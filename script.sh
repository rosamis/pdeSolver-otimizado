#!/bin/bash
# #remove todos os arquivos gerados anteriormente para o novo grafico
export PATH="/home/soft/likwid/bin:/home/soft/likwid/sbin:${PATH}"
export LD_LIBRARY_PATH=/home/soft/likwid/lib

opcao=0
while [ $opcao -ne 4 ]
do
	echo " ==================================="
	echo "| Escolha uma opção:                |"
	echo "| 1 - Rodar testes                  |"
	echo "| 2 - Limpar arquivos *txt *gnu dir |"
	echo "| 3 - Listar arquivos               |"
	echo "| 4 - Finalizar Script              |"
	echo " ==================================="

	read opcao

	case $opcao in 
		1)  
			echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
			echo "-------------------"
			echo " Modo: PERFORMANCE "
			echo "-------------------"
			tipoFuncao=(gauss normaL2);
			# tipoFuncao=(eliminacaoGauss gaussJacobi);
            for funcao in "${tipoFuncao[@]}"
            do
                #verifica se o diretório da função já foi criado
                if [ -e "./$funcao" ]
                then
                    echo "Diretório já existe";
                else
                    echo "Criando o diretório";
                    mkdir ./$funcao;
                fi
                #percorre no vetor o que eu quero medir do trabalho 
                dado=L2CACHE;
                valores=(32 50 64 100 128 200 256 300 400 512 1000 1024 2000 2048 3000 4000 4096 5000 10000);
                #coloca em um vetor todos os tamanhos de matriz que eu quero
                echo "Medindo $dado da funcao $funcao, aguarde ...";
                #crio um arquivo com o nome dado e do grupo para buscar as informacoes
                touch dados$dado$funcao.txt
                #crio um arquivo com o nome dado e do grupo para colocar as informações filtradas
                touch plot$dado$funcao.txt
                #compilo com ifdef para não precisar ter + de 1 arquivo.c
                make -B $funcao
                for var in "${valores[@]}" 
                do
                	echo "tamanho = $var na função $funcao testando o dado $dado";    		 	
        	        #roda o likwid e joga os dados num arquivo txt separado 
	                likwid-perfctr -C 3 -g $dado -m -f ./pdeSolver -nx $var -ny $var -i 10 >> dados$dado$funcao.txt
                done

                echo "Gerando arquivo plot da função $funcao do dado $dado";
                python3 script.py dados$dado$funcao.txt >> plot$dado$funcao.txt
                touch gnu$dado$funcao.gnu
                echo "set terminal jpeg" >> gnu$dado$funcao.gnu
                echo "set title 'Função $funcao testando $dado'" >> gnu$dado$funcao.gnu
                echo "set key above" >> gnu$dado$funcao.gnu
                echo "set xlabel 'TAMANHO'" >> gnu$dado$funcao.gnu
                echo "set ylabel 'INDICADOR DO TESTE (Escala log)'" >> gnu$dado$funcao.gnu
                echo "set logscale y" >> gnu$dado$funcao.gnu
                echo "set output 'imagem$dado$funcao.jpeg'" >> gnu$dado$funcao.gnu
                echo "plot 'plot$dado$funcao.txt' title 'Data Cache Miss Ratio T2' with lines, 'plot0$dado$funcao.txt' title 'Data Cache Miss Ratio T1' with lines" >> gnu$dado$funcao.gnu
                gnuplot -e "load 'gnu$dado$funcao.gnu'; exit"
                mv imagem$dado$funcao.jpeg $funcao/	
                
                dado=L3;
                valores=(32 50 64 100 128 200 256 300 400 512 1000 1024 2000 2048 3000 4000 4096 5000 10000);
                #coloca em um vetor todos os tamanhos de matriz que eu quero
                echo "Medindo $dado da funcao $funcao, aguarde ..."
                #crio um arquivo com o nome dado e do grupo para buscar as informacoes
                touch dados$dado$funcao.txt
                #crio um arquivo com o nome dado e do grupo para colocar as informações filtradas
                touch plot$dado$funcao.txt
                #compilo com ifdef para não precisar ter + de 1 arquivo
                for var in "${valores[@]}" 
                do
                    echo "tamanho = $var na função $funcao testando o dado $dado";  	
                    #roda o likwid e joga os dados num arquivo txt separado 
                    likwid-perfctr -C 3 -g $dado -m -f ./pdeSolver -nx $var -ny $var -i 10 >> dados$dado$funcao.txt
                done    

                echo "Gerando arquivo plot da função $funcao do dado $dado";
                python3 script.py dados$dado$funcao.txt >> plot$dado$funcao.txt
                touch gnu$dado$funcao.gnu
                echo "set terminal jpeg" >> gnu$dado$funcao.gnu
                echo "set title 'Função $funcao testando $dado'" >> gnu$dado$funcao.gnu
                echo "set key above" >> gnu$dado$funcao.gnu
                echo "set xlabel 'TAMANHO'" >> gnu$dado$funcao.gnu
                echo "set ylabel 'INDICADOR DO TESTE (Escala log)'" >> gnu$dado$funcao.gnu
                echo "set logscale y" >> gnu$dado$funcao.gnu
                echo "set output 'imagem$dado$funcao.jpeg'" >> gnu$dado$funcao.gnu
                echo "plot 'plot$dado$funcao.txt' title 'Memory Bandwidth [MBytes/s] T2' with lines, 'plot0$dado$funcao.txt' title 'Memory Bandwidth [MBytes/s] T1' with lines" >> gnu$dado$funcao.gnu
                gnuplot -e "load 'gnu$dado$funcao.gnu'; exit"
                mv imagem$dado$funcao.jpeg $funcao/	

                dado=FLOPS_DP;
                valores=(32 50 64 100 128 200 256 300 400 512 1000 1024 2000 2048 3000 4000 4096 5000 10000);
                #coloca em um vetor todos os tamanhos de matriz que eu quero
                echo "Medindo $dado da funcao $funcao, aguarde ..."
                #crio um arquivo com o nome dado e do grupo para buscar as informacoes
                touch dados$dado$funcao.txt
                #crio um arquivo com o nome dado e do grupo para colocar as informações filtradas
                touch plot$dado$funcao.txt
                touch plotAVX$dado$funcao.txt
                #compilo com ifdef para não precisar ter + de 1 arquivo
                for var in "${valores[@]}" 
                do
                    echo "tamanho = $var na função $funcao testando o dado $dado";  	
                    #roda o likwid e joga os dados num arquivo txt separado 
                    likwid-perfctr -C 3 -g $dado -m -f ./pdeSolver -nx $var -ny $var -i 10 >> dados$dado$funcao.txt
                done
                python3 script.py dados$dado$funcao.txt DP >> plot$dado$funcao.txt
                echo 'Rodei pro FLOPS_DP'
		      	python3 script.py dados$dado$funcao.txt AVX >> plotAVX$dado$funcao.txt
                echo 'Rodei pro AVX FLOPS_DP'

                echo "Gerando arquivo plot da função $funcao do dado $dado";

                touch gnu$dado$funcao.gnu
                echo "set terminal jpeg" >> gnu$dado$funcao.gnu
                echo "set title 'Função $funcao testando $dado'" >> gnu$dado$funcao.gnu
                echo "set key above" >> gnu$dado$funcao.gnu
                echo "set xlabel 'TAMANHO'" >> gnu$dado$funcao.gnu
                echo "set ylabel 'INDICADOR DO TESTE (Escala log)'" >> gnu$dado$funcao.gnu
                echo "set logscale y" >> gnu$dado$funcao.gnu
                echo "set output 'imagem$dado$funcao.jpeg'" >> gnu$dado$funcao.gnu
                echo "plot 'plot$dado$funcao.txt' title 'FLOPS DP T2' with lines, 'plotAVX$dado$funcao.txt' title 'AVX FLOPS DP T1' with lines, 'plot0$dado$funcao.txt' title 'FLOPS DP T1' with lines, 'plot0AVX$dado$funcao.txt' title 'AVX FLOPS DP T1' with lines"  >> gnu$dado$funcao.gnu
                gnuplot -e "load 'gnu$dado$funcao.gnu'; exit"
                mv imagem$dado$funcao.jpeg $funcao/	

                dado=TEMPO;
                valores=(32 50 64 100 128 200 256 300 400 512 1000 1024 2000 2048 3000 4000 4096 5000 10000);
                touch plot$dado$funcao.txt 
                make purge
                make -B $funcao$dado
                for var in "${valores[@]}" 
                do
                    echo "tamanho = $var na função $funcao testando o dado $dado";  	
                    #roda o likwid e joga os dados num arquivo txt separado 
                    ./pdeSolver -nx $var -ny $var -i 10 >> plot$dado$funcao.txt
                done
                echo "Gerando arquivo plot da função $funcao do dado $dado";
                touch gnu$dado$funcao.gnu
                echo "set terminal jpeg" >> gnu$dado$funcao.gnu
                echo "set title 'Função $funcao testando $dado em Milisegundos'" >> gnu$dado$funcao.gnu
                echo "set key above" >> gnu$dado$funcao.gnu
                echo "set xlabel 'TAMANHO'" >> gnu$dado$funcao.gnu
                echo "set ylabel 'INDICADOR DO TESTE.'" >> gnu$dado$funcao.gnu
                echo "set logscale" >> gnu$dado$funcao.gnu
                echo "set output 'imagem$dado$funcao.jpeg'" >> gnu$dado$funcao.gnu
                echo "plot 'plot$dado$funcao.txt' title 'T2' with lines, 'plot0$dados$funcao.txt' title 'T1' with lines" >> gnu$dado$funcao.gnu
                gnuplot -e "load 'gnu$dado$funcao.gnu'; exit"
                mv imagem$dado$funcao.jpeg $funcao/	
            done
            echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
            echo "-----------------"
            echo " Modo: POWERSAVE "
            echo "-----------------"
            ;;
        2)
            rm *.txt
            rm *.gnu
            rm gauss/ -R
            rm normaL2/ -R
            ;;
    
        3) 
            ls
            ;;

        4) 
            ;;
    esac
done


compile:
	bash compile_2.sh
	
user_man:
	pdflatex pz.tex

in_instruct:
	pdflatex instruct_install.tex
	
license:
	pdflatex licence.tex

docs: user_man in_instruct license
	
configure:
	bash configure.sh
	
install: 
	# Создание директории в /etc
	sudo mkdir -p /etc/bz_conf
	# Создание директории для логов
	sudo mkdir -p /var/log/bz_logs
	sudo chmod -R 777 /var/log/bz_logs
	# Перемещение исполняемого и настреочного файлов в bz_conf
	sudo mv 2 /etc/bz_conf
	sudo mv install.conf /etc/bz_conf
	# Создание символической ссылки на исполняемый файл
	sudo ln -s 2 /usr/bin
	
start:
	bash start.sh
clear_tmp: 
	bash clear_tmp.sh
	
clear_config:
	rm -f install.conf
	
clear_compile:
	rm -f 2

clear: clear_tmp clear_compile

uninstall:
	bash uninstall.sh 1

remove_all:
	bash uninstall.sh 2
	
	sudo rm -rf /etc/bz_conf
	sudo rm -rf /var/log/bz_logs
	sudo unlink /usr/bin/2
	rm -f /home/vboxuser/bz/pz.pdf
	rm -f /home/vboxuser/bz/pz.aux
	rm -f /home/vboxuser/bz/pz.log
	rm -f /home/vboxuser/bz/instruct_install.pdf
	rm -f /home/vboxuser/bz/instruct_install.aux
	rm -f /home/vboxuser/bz/instruct_install.log
	rm -f /home/vboxuser/bz/licence.pdf
	rm -f /home/vboxuser/bz/licence.aux
	rm -f /home/vboxuser/bz/licence.log
	rm -f /home/vboxuser/bz/texput.log
	
all: compile configure install docs


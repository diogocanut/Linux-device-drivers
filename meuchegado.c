
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include <asm/uaccess.h>

/*
  Device driver com intuíto de printar a frase "E aí meu chegado", quando requisitado em /dev/chegado
 */

static ssize_t chegado_read(struct file * file, char * buf, 
			  size_t count, loff_t *ppos){

	char *chegado_str = "E aí meu chegado!\n";
	int len = strlen(chegado_str);
	/*
	 * É apenas suportado a leitura da string inteira de uma vez.
	 */
	if (count < len)
		return -EINVAL;
	/*
	 Se a posição do arquivo é não zero, então a string não foi lida inteira.
	 */
	if (*ppos != 0)
		return 0;
	/*
	 Copia a string para o buffer providenciado pelo usuário e checa se
	 o mesmo possui permissao para escrita.
	 */

	if (copy_to_user(buf, chegado_str, len))
		return -EINVAL;
	/*
	 Mostra o usuário o tamanho da string escrita.
	 */
	*ppos = len;

	return len;
}

/*
 A unica operação utilizada é a leitura.
 */

static const struct file_operations chegado_fops = {
	.owner		= THIS_MODULE,
	.read		= chegado_read,
};

static struct miscdevice chegado_dev = {
	/*
	Não importa qual o minor numb, então pegamos qualquer um.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 Nomeia /dev/chegado
	 */
	"chegado",

	&chegado_fops
};

static int __init
chegado_init(void){

	int ret;
	ret = misc_register(&chegado_dev);
	if (ret)
		printk(KERN_ERR
		       "Erro ao registrar \"E aí meu chegado!\" misc device\n");

	return ret;
}

module_init(chegado_init);

static void __exit
chegado_exit(void){

	misc_deregister(&chegado_dev);
}

module_exit(chegado_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("knut");
MODULE_DESCRIPTION("\"E aí meu chegado!\" module");
MODULE_VERSION("dev");

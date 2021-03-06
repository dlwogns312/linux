#include "output_process.h"

void output_process(int shm_output)
{
    printf("Output_process successfully Loaded!\n");

    int check_terminate=0;
    
    SHM_OUTPUT* data_out=(SHM_OUTPUT*)shmat(shm_output,(char*)NULL,0);

    while(!check_terminate)
    {
        //sleep(1);
        
        //printf("output\n");
        if(data_out->check_terminate)
        {
            check_terminate=1;
        }

        semop(sem_id,&p[1],1);
        device_fnd(data_out->fnd_data);
        device_led(data_out->led);
        device_lcd(data_out->text_data);
        device_dot(data_out->display_dot);
        semop(sem_id,&v[1],1);
        
        usleep(100000);
    }
    shmdt((char*)data_out);

    printf("Output_process out!\n");

    return;
}

void device_fnd(int fnd_data)
{
    unsigned char save[4];
    int i;
    int dev;
    char* fnd_pos="/dev/fpga_fnd";
    dev=open(fnd_pos,O_RDWR);

    if(dev<0)
    {
        printf("Device open error: %s\n",fnd_pos);
        exit(1);
    }

    for(i=3;i>=0;i--)
    {
        save[i]=fnd_data%10;
        fnd_data/=10;
    }
   
    unsigned char for_write=write(dev,&save,4);

    if(for_write<0)
    {
        printf("Write Error!\n");
        return;
    }

    close(dev);
}

void device_led(unsigned char led)
{
    int fd,i;
    unsigned long *fpga_addr = 0;
    unsigned char *led_addr =0;

    //printf("%d\n",led);
    fd=open("/dev/mem",O_RDWR|O_SYNC);
    if(fd<0)
    {
        perror("/dev/mem open error");
        exit(1);
    }

    fpga_addr = (unsigned long *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FPGA_BASE_ADDRESS);
	if (fpga_addr == MAP_FAILED)
	{
		printf("mmap error!\n");
		close(fd);
		exit(1);
	}
	
	led_addr=(unsigned char*)((void*)fpga_addr+LED_ADDR);

	//write led
	*led_addr=led; 
	
	munmap(led_addr, 4096);
	close(fd);

    usleep(1000);
	return ;
}

void device_dot(unsigned char data[10])
{
    int fd;
    char* input_name="/dev/fpga_dot";
    fd=open(input_name, O_WRONLY);
    if(fd<0)
    {
        printf("Device open error : %s\n",input_name);
        exit(1);
    }

    write(fd,data,10);
    close(fd);
}

void device_lcd (unsigned char data[32])
{
    int fd,i;
    unsigned char str[32];
    char *input_name="/dev/fpga_text_lcd";
    fd=open(input_name,O_WRONLY);
    if(fd<0)
    {
        printf("Device open error : %s\n",input_name);
        exit(1);
    }

    write(fd,data,32);
    close(fd);
}
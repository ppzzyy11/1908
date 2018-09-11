#include<stdio.h>
#include<stdlib.h>

struct Data{
    char source[20];
    char name[10];
    int color;//色度
    float ph;//ph 值
    int Ga;//gai钙
    int Mg;//镁
    int Na;//钠
    float radio;//放射性
    struct Data* next;
};//用链表
//同时我们会做一个fake head

struct Data* head ;
/*struct Data* head = (struct Data*) malloc(sizeof(struct Data));*/

struct Data* NewDataNode()
{
    struct Data* node=(struct Data*) malloc(sizeof(struct Data));
    for(int i=0;i<20;i++) node->source[i]=0;
    for(int i=0;i<10;i++) node->name[i]=0;
    node->color=-1;
    node->ph=-1;
    node->Ga=-1;
    node->Mg=-1;
    node->Na=-1;
    node->radio=-1;
    node->next=NULL;
    return node;
}

void OutputData(struct Data* node)
{
    fprintf(stdout,"%s,%s,%d,%.1f,%d,%d,%d,%.3f\n",node->source,node->name,node->color,node->ph,node->Ga,node->Mg,node->Na,node->radio);
    /*fprintf(stdout,"%s,%s,%d,%.1f,%d,%d,%d,%.3f\n",node->source,node->name,node->color,node->ph,node->Ga,node->Mg,node->Na,node->radio);*/
    /*printf("%s,%s,%d,%.1f,%d,%d,%d,%.3f\n",node->source,node->name,node->color,node->ph,node->Ga,node->Mg,node->Na,node->radio);*/
}

int After(struct Data* l,struct Data * r)//l be after r?
{
    if(l->radio>r->radio) return 1;
    if(l->radio<r->radio) return 0;
    if(l->Mg<r->Mg) return 1;
    if(l->Mg>r->Mg) return 0;
    return 1;
}

void InsertNode(struct Data* node)
{
    struct Data* tmp=head;
    while(tmp->next&&After(node,tmp->next))
    {
        tmp=tmp->next;
    }
    struct Data* next=tmp->next;
    tmp->next=node;
    node->next=next;
}

int ValidString(char ch)
{
    if(ch>='a'&&ch<='z') return 1;
    if(ch>='A'&&ch<='Z') return 1;
    if(ch>='0'&&ch<='9') return 1;
    if(ch==13||ch==10) return 1;
    return 0;
}

int eof=0;

struct Data* ReadData(FILE* fp)
{
    struct Data* node=NewDataNode();

    char ch;

    int i=0;
    while(i<20&&(ch=fgetc(fp))!=',')
    {
        if(ch==-1)
        {
            free(node);
            eof=1;
            return NULL;
        }
        if(!ValidString(ch))
        {
            fprintf(stderr,"读取错误\n");
            free(node);
            return NULL;
        }
        /*printf("%dth ch:%d\n",i,ch);*/
        if(ch!='\n'&&ch!=13&&ch!=10)
        {
            node->source[i]=ch;
            i++;
        }
    }
    if(i==20||i==0)
    {
        fprintf(stderr,"读取错误\n");
        free(node);
        return NULL;
    }
    /*printf("1\n");*/

    i=0;
    while(i<10&&(ch=fgetc(fp))!=',')
    {
        node->name[i]=ch;
        i++;
    }
    if(i==0||i==10)
    {
        fprintf(stderr,"读取错误\n");
        /*printf("\n2\n");*/
        free(node);
        return NULL;
    }

    int rd=fscanf(fp,"%d,%f,%d,%d,%d,%f",&node->color,&node->ph,&node->Ga,&node->Mg,&node->Na,&node->radio);
    if(rd!=6)
    {
        fprintf(stderr,"读取错误\n");
        /*printf("\n3\n");*/
        free(node);
        return NULL;
    }

    InsertNode(node);

}

int Better(struct Data* l, struct Data* r)//l's color ,ph ,Ga , Na, Mg should be less that r
{
    if(l->color>r->color) return 0;
    if(l->Ga>r->Ga) return 0;
    if(l->Na>r->Na) return 0;
    if(l->Mg>r->Mg) return 0;
    return 1;
}

void BetterOutput(struct Data* node)
{
    struct Data* tmp=head->next;
    while(tmp)
    {
        if(Better(tmp,node))
        {
            OutputData(tmp);
        }
        tmp=tmp->next;
    }
}

int main()
{
    head = (struct Data*) malloc(sizeof(struct Data));
    FILE *fp=NULL;//打开文件
    fp=fopen("samples.txt","r");
    if(fp==NULL)
    {
        fprintf(stderr,"File open error!\n");
        return 1;
    }

    //下面开始读取文件并存储到链表中

    struct Data* tmp=head->next;
    while(!feof(fp))
    {
        if(ReadData(fp)==NULL)
        {
            if(eof==1)//read the End Of File
            {
                eof=0;
            }else
            {
                /*GG*/
                tmp=head;
                while(tmp)
                {
                    struct Data* next=tmp->next;
                    free(tmp);
                    tmp=next;
                }
                fclose(fp);
                return 1;
            }
        }
    }

    /*tmp=head->next;*/
    /*while(tmp)*/
    /*{*/
        /*Data* next=tmp->next;*/
        /*OutputData(tmp);*/
        /*tmp=next;*/
    /*}*/

    //下面这个循环是在进行查询和搜索
    while(1)
    {
        printf("\n\n输入color ph Ga Mg Na的最大值(q开头结束)\n");
        struct Data* node= NewDataNode();
        char ch=0;
        ch=getchar();
        if(ch=='q')
            break;
        node->color=ch-'0';
        while((ch=getchar())!=' ')
        {
            node->color=node->color*10+ch-'0';
        }
        fscanf(stdin,"%f%d%d%d",&node->ph,&node->Ga,&node->Mg,&node->Na);
        /*OutputData(node);*/
        while(getchar()!='\n')
        {
            ;
        }
        /*fscanf(stdin,"%d%f%d%d%d",&node->color,&node->ph,&node->Ga,&node->Mg,&node->Na);*/
        BetterOutput(node);
        free(node);
    }

    //free malloc memory
    tmp=head;
    while(tmp)
    {
        struct Data* next=tmp->next;
        free(tmp);
        tmp=next;
    }
    fclose(fp);
    return 0;
}

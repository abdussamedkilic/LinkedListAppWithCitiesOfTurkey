#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///komşuluklar ilişkisini tutacak , başlangıçı struct city içerisindeki *firstn olucak
struct neighbours
{
    int plate;
    char name[15];
    struct neighbours *next;
};

/// şehir ilişkilerini tutacak , komşu sayısı nasıl hesaplanacak kafamda çözemedim
struct city
{
    int plate;
    int neighbourNum;
    char name[17];
    char bolge[3];
    struct city *next;
    struct city *before;
    struct neighbours *firstn;
};

///yeni satır atıyor
void newLine()
{
    printf("\n");
}

void PrintStar()
{
    int i;
    for(i = 0; i < 50; ++i)
    {
        printf("*");
    }
    printf("\n");
}

void PrintSharp()
{
    int i;
    for(i = 0; i < 50; ++i)
    {
        printf("#");
    }
    printf("\n");
}

///fonksiyon adı yanlış olmuş showStart olucaktı :P
void showStar(int s1, int s2)
{
    int i,j;
    for(i=0; i<s1; i++)
    {
        for(j=0; j<s2; j++)
        {
            printf("*");
        }
        newLine();
    }

}

///String karşılaştırması için ascii değerlerini hesaplıyor , uppercase veya lowercase önemsenmeden
///HATA OLABİLİR METOT ÜZERİNE DAHA ÇOK KAFA YOR
int findAsciiValueFromString(char *arr, char *ary)
{
    int asciiarr = 0;
    int asciiary = 0;
    int i;
    int control = 1;

    for(i=0;  arr[i] != '\0' || ary[i] != '\0'; i++)
    {
        asciiarr = ((int)arr[i])%32;
        asciiary =((int)ary[i])%32;
        if(asciiarr != asciiary)
        {
            control = 0;
        }

    }

    return control;
}

/// şehir ilişkilerinde ilk şehri ve son şehri tutuyor
struct city *first = NULL;
struct city  *last =NULL;
/// yeni şehir ekleme metotu
int addNewCity(int p, char *n, char *b )
{

    /// unique isterlerle aynı şehir var mı yok mu kontrlü yapıyor
    struct city *control = first;
    int cntrl = 0;
    while(control!=NULL)
    {
        if((findAsciiValueFromString(control->name, n) == 1) && control->plate == p)
        {
            newLine();
            printf("Şehir adi : %s \n",n);
            printf("şehir plakası : %d \n",p);
            printf("Şehirler listesinde bu iki koşulala aynı şehir vardır , şehir eklenemiyor !!! \n");
            showStar(1,20);
            return -3;
        }
        if(findAsciiValueFromString(control->name, n) == 1)
        {

            newLine();
            printf("şehir adi : %s ",n);
            newLine();
            printf(" Şehirler listesinde bu isimde bir şehir vardır");
            printf(" , şehir eklenemiyor !!! \n");
            showStar(1,20);
            return -1;
        }
        if(control->plate == p)
        {

            newLine();
            printf("şehir plakası : %d",p);
            newLine();
            printf("Şehirler listesinde bu plakada bir şehir vardır");
            printf(" , şehir eklenemiyor !!! \n");
            showStar(1,20);
            return -2;

        }

        control = control->next;
    }
    /// eklenecek şehri yapılandırıyor
    struct city *added = (struct city*)malloc(sizeof(struct city));
    added->plate=p;
    strcpy(added->name, n);
    strcpy(added->bolge, b);
    added->firstn = NULL;
    added->neighbourNum = 0;

    /// şehir yoksa ilk şehri ekliyor
    if (first == NULL)
    {
        first = added;
        last = added;
        first->before = NULL;
        last->next = NULL;

    }
    /// eklenecek şehrin plakası büyükse sona ekliyor
    if(last->plate < added->plate)
    {
        last->next = added;
        added->before = last;
        last = added;
        last->next = NULL;

    }
    ///eklenecek şehrin plakası küçükse 1 şehir varsa başa ekliyor ,
    ///2'den fazla şehir varsa kontrol yapıp ya başa ya da araya ekliyor
    if(last->plate > added->plate)
    {

        if(first->next == NULL && last->before ==NULL || first->plate > added->plate)
        {
            first->before = added;
            added->next = first;
            added->before = NULL;
            first = added;

        }

        else
        {
            if(last->before->plate > added->plate)
            {
                last->before->before->next = added;
                added->next = last->before;
                added->before = last->before->before;
            }
            else
            {
                added->before = last->before;
                added->next = last;
                last->before->next = added;
                last->before = added;
            }
        }
    }
    return 1;

}

/// şehir adından şehri gösteren pointer'ı buluyor , yoksa null dönderiyor
struct city *findCityFromName(char *name)
{
    int control;
    struct city *temp = first;
    while(temp!=NULL)
    {
        control = findAsciiValueFromString(temp->name, name);
        if(control == 1)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/// plakadan şehri gösteren pointer'ı buluyor yoksa null dönderiyor
struct city *findCityFromPlate(int plate)
{

    struct city *temp = first;

    while(temp!=NULL)
    {

        if(temp->plate == plate)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/// yeni komu ekliyor
int addNeighbours(struct city *control, int p, char *n)
{
    /// aynı isimden veya plakadan komşu eklenmesini önlüyor;
    struct neighbours *test = control->firstn;
    int cntrl = 0;
    while(test != NULL)
    {
        if((findAsciiValueFromString(test->name, n) == 1) && test->plate == p)
        {
            printf("[%d] [%s] şehrinde %d %s ile aynı komşuluğa sahiptir \n",control->plate,control->name,p,n);
            printf("Komşuluk eklenemiyor !!! \n");
            showStar(1,20);
            return -3;
        }
        if(findAsciiValueFromString(test->name, n) == 1)
        {

            newLine();
            printf("[%d] [%s] şehrinde %s ismiyle aynı komşuluğa sahiptir \n",control->plate,control->name,n);
            printf("Komşuluk eklenemiyor !!!\n");
            showStar(1,20);
            return -1;
            cntrl++;
        }
        if(test->plate == p)
        {

            newLine();
            printf("[%d] [%s] şehrinde %d plakasıyla aynı komşuluğa sahiptir \n",control->plate,control->name,p);
            printf("Komşuluk eklenemiyor !!! \n");
            showStar(1,20);
            return -2;
            cntrl++;
        }

        test = test->next;
    }

    struct neighbours *added = (struct neighbours *)malloc(sizeof(struct neighbours ));
    strcpy(added->name, n);
    added->plate =p;

    if(control->firstn == NULL)
    {
        control->firstn = added;
        added->next = NULL;
        control->neighbourNum = 1;
    }

    struct neighbours *temp = control->firstn;

    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    if(temp->plate < added->plate)
    {
        temp->next = added;
        added->next = NULL;
        control->neighbourNum++;
    }

    else if(temp->plate > added->plate)
    {
        if(control->firstn->next == NULL || control->firstn->plate > added->plate)
        {
            added->next = control->firstn;
            control->firstn = added;
            control->neighbourNum++;

        }

        else if(control->firstn->plate < added->plate)
        {
            struct neighbours *tmp = control->firstn;
            while(tmp != temp)
            {
                if(tmp->plate < added->plate)
                {
                    if(tmp->next->plate > added->plate)
                    {
                        break;
                    }
                }
                tmp = tmp->next;
            }
            added->next = tmp->next;
            tmp->next = added;
            control->neighbourNum++;

        }
    }
    return 1;
}

/// aynı bölgedeki şehirlerin bilgilerini bastırıyor;
void ListCityInSameRegion(char *region, FILE *fp )
{
    struct city *temp = first;
    printf("[%s] bölgesindeki şehirler gösteriliyor : ... \n",region);
    fprintf(fp,"[%s] bölgesindeki şehirler gösteriliyor : ... \n",region);
    int control = 0;
    while(temp->next != NULL)
    {
        if(findAsciiValueFromString(temp->bolge, region) == 1)
        {
            control++;
            printf("{ [%d] [%s] [%s] [%d] } --->",temp->plate,temp->name, temp->bolge,temp->neighbourNum);
            fprintf(fp,"{ [%d] [%s] [%s] [%d] } -->",temp->plate,(temp->name),(temp->bolge),temp->neighbourNum);
            ListToNeighbours(temp,fp);
            newLine();
        }
        temp = temp->next;
    }
    if(control == 0)
    {
     printf("Girdiğiniz bölge (%s) hatalı veya bu bölgede hiç şehir yoktur \n",region);
     fprintf(fp,"Girdiğiniz bölge (%s) hatalı veya bu bölgede hiç şehir yoktur \n\n",region);

    }
    fprintf(fp,"\n");
    newLine();

}

/// ListCityToNeighboursNumber metotunun menüsü
void MenuOfListCityToNeighboursNumber()
{
    PrintStar();
    printf("Nasil bir aralik istiyorsunuz : ... \n");
    printf("1) girdiğiniz sayidan daha az,eşit sayıda komşuya sahip şehirleri listeler \n");
    printf("2) girdiğiniz sayidan daha fazla, sayıda komşuya sahip şehirleri listeler \n");
    printf("3) iki sayı girmenizi ister , girdiğiniz sayılarin arasında kalan komşu sayısına sahip şehirleri listeler\n");
    printf("{not : eğer enter tuşunu girerseniz işlem baştan başlıyacaktır} \n");
    PrintStar();
}
/// kullanıcıdan girilen koşullara göre sınırlandırılmış alandaki komşu sayısına sahip şehirleri listeler;
void ListCityToNeighboursNumber(FILE *fp)
{
    struct city *temp = first;
    int key;

    MenuOfListCityToNeighboursNumber();
    printf("seçiminiz ? --> ");
    scanf("%d",&key);

    if(key == 1)
    {
        int control = 0;
        int bignum;
        printf("sayi giriniz : ");
        scanf("%d",&bignum);
        fprintf(fp,"%d sayısından eşit veya daha az komşusu bulunan şehirler listeleniyor : \n",bignum);
        fprintf(fp,"\n");
        newLine();
        while(temp->next != NULL)
        {
            if(temp->neighbourNum <= bignum)
            {
                printf("{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
                fprintf(fp,"{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
                ListToNeighbours(temp,fp);
                fprintf(fp,"\n");
                newLine();
                control++;
            }
            temp=temp->next;
        }
        newLine();
        fprintf(fp,"\n");
        if(control == 0)
        {
            printf("girilen (%d) sayidan daha az,eşit komşuya sahip şehir bulunamamıştır \n",bignum);
            fprintf(fp,"girilen (%d) sayidan daha az,eşit komşuya sahip şehir bulunamamıştır \n",bignum);
        }
    }

    if(key == 2)
    {
        int control = 0;
        int smallnum;
        printf("sayi giriniz : ");
        scanf("%d",&smallnum);
        fprintf(fp,"%d sayısından daha çok komşuluğa sahip şehirler listeleniyor : \n",smallnum);
        fprintf(fp,"\n");
        newLine();
        while(temp->next != NULL)
        {
            if(temp->neighbourNum > smallnum)
            {
                printf("{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
                fprintf(fp,"{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
                ListToNeighbours(temp,fp);
                control++;
                newLine();
            }
            temp=temp->next;
        }
        newLine();
        if(control == 0)
        {
            printf("girilen (%d) sayidan daha fazla komşuya sahip şehir bulunamamıştır \n",smallnum);
            fprintf(fp,"girilen (%d) sayidan daha fazla komşuya sahip şehir bulunamamıştır \n",smallnum);
        }
    }

    if(key == 3)
    {
        int control = 0;
        int smallnum;
        int bignum;
        printf("2 sayi giriniz : ");
        scanf("%d %d",&smallnum,&bignum);
        newLine();
        if(bignum < smallnum)
        {
            int tmp = bignum;
            bignum = smallnum;
            smallnum = tmp;
        }
        fprintf(fp,"%d den daha çok ve %d den eşit veya daha az komşuluğa sahip şehirler listeleniyor : \n",smallnum,bignum);
        while(temp->next != NULL)
        {
            if(temp->neighbourNum > smallnum && temp->neighbourNum <= bignum)
            {
                printf("{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
                fprintf(fp,"{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
                ListToNeighbours(temp,fp);
                newLine();
                control++;
            }
            temp=temp->next;
        }
        newLine();
        fprintf(fp,"\n");
        if(control == 0)
        {
            printf("girilen (%d) sayisindan daha fazla ve (%d) sayisindan daha az,eşit komşuya sahip şehir bulunamamıştır \n",smallnum,bignum);
            fprintf(fp,"girilen (%d) sayisindan daha fazla ve (%d) sayisindan daha az,eşit komşuya sahip şehir bulunamamıştır \n",smallnum,bignum);
        }
    }
}

///Ortak Komşuya veya komşulara sahip ve belirli bir aralıktaki komşu sayısına sahip şehirleri sıralar
void ListCityHaveSameNeighbours(FILE *fp)
{
    struct city *ctemp = first;
    struct neighbours *ntemp ;
    int bignum, smallnum;
    printf("iki sayi giriniz : ");
    printf("\n 1. sayi : ");
    scanf("%d",&smallnum);
    printf("\n 2. sayi : ");
    scanf("%d",&bignum);
    newLine();
    if(bignum < smallnum)
    {
        int tmp = bignum;
        bignum = smallnum;
        smallnum = tmp;
    }
    fprintf(fp,"%d den daha çok ve %d den eşit veya daha az komşuluğa sahip ve ",smallnum,bignum);
    int key;
    printf("kac tane ortak komşuya sahip şehir gireceksiniz (maks 3 , min 1) : ");
    scanf("%d",&key);
    if(key==1)
    {
        char one[17];
        printf("araştırılmasını istediğiniz ortak komşuyu giriniz : ");
        scanf("%s",&one);
        fprintf(fp,"[%s] komşu olan şehirler listeleniyor : \n",one);
        int control = 0;
        int isfind = 0;
        newLine();
        fprintf(fp,"\n");
        while(ctemp != NULL)
        {
            if(ctemp->neighbourNum > smallnum && ctemp->neighbourNum <= bignum)
            {
                ntemp = ctemp->firstn;
                while(ntemp != NULL)
                {
                    if(findAsciiValueFromString(ntemp->name, one) == 1)
                    {
                        control++;
                    }
                    ntemp = ntemp->next;
                }
            }
            if(control == 1)
            {
                printf("{ [%d] [%s] [%s] } --> ",ctemp->plate, ctemp->name, ctemp->bolge);
                fprintf(fp,"{ [%d] [%s] [%s] } --> ",ctemp->plate, ctemp->name, ctemp->bolge);
                ListToNeighbours(ctemp,fp);
                newLine();
                fprintf(fp,"\n");
                isfind++;
            }
            ctemp = ctemp->next;
            control = 0;
        }
        if(isfind == 0)
        {
            printf("%d-%d aralığında ve %s'e komşu şehir",smallnum,bignum,one);
            fprintf(fp,"%d-%d aralığında ve %s'e komşu şehir",smallnum,bignum,one);
            printf(" koşullarına uygun bir şehir yoktur\n");
            fprintf(fp," koşullarına uygun bir şehir yoktur \n");
        }
    }
    if(key==2)
    {
        char one[17];
        char two[17];
        printf("araştırılmasını istediğiniz  2 ortak komşunun adını giriniz : ");
        printf("\n 1. komşu : ");
        scanf("%s",&one);
        printf("\n 2. komşu : ");
        scanf("%s",&two);
        fprintf(fp,"[%s] ve [%s] 'ye komşu olan şehirler listeleniyor : \n",one,two);
        int control = 0;
        int isfind = 0;
        newLine();
        fprintf(fp,"\n");
        while(ctemp != NULL)
        {
            if(ctemp->neighbourNum > smallnum && ctemp->neighbourNum <= bignum)
            {
                ntemp = ctemp->firstn;
                while(ntemp != NULL)
                {
                    if(findAsciiValueFromString(ntemp->name, one) == 1 )
                    {
                        control++;
                    }
                    if(findAsciiValueFromString(ntemp->name, two) == 1)
                    {
                        control++;
                    }
                    ntemp = ntemp->next;
                }

            }
            if(control == 2)
            {
                printf("{ [%d] [%s] [%s] } --> ",ctemp->plate, ctemp->name, ctemp->bolge);
                fprintf(fp,"{ [%d] [%s] [%s] } --> ",ctemp->plate, ctemp->name, ctemp->bolge);
                ListToNeighbours(ctemp,fp);
                newLine();
                fprintf(fp,"\n");
                isfind++;
            }
            ctemp = ctemp->next;
            control = 0;
        }
        if(isfind == 0)
        {
            printf("%d-%d aralığında ve %s'e ve %s'e komşu şehir",smallnum,bignum,one,two);
            fprintf(fp,"%d-%d aralığında ve %s'e ve %s'e komşu şehir",smallnum,bignum,one,two);
            printf(" koşullarına uygun bir şehir yoktur\n");
            fprintf(fp," koşullarına uygun bir şehir yoktur\n");
        }
    }
    if(key==3)
    {
        char one[17], two[17], three[17];
        printf("araştırılmasını istediğiniz 3 komşunun adını giriniz : ");
        printf("\n 1. komşu adı : ");
        scanf("%s",&one);
        printf("\n 2.komşu adı : ");
        scanf("%s",&two);
        printf("\n 3. komşu adı : ");
        scanf("%s",&three);
        fprintf(fp,"[%s] , [%s] ve [%s] 'e komşu olan şehirler listeleniyor : \n",one,two,three);
        int control = 0;
        int isfind = 0;
        while(ctemp != NULL)
        {
            if(ctemp->neighbourNum > smallnum && ctemp->neighbourNum <= bignum)
            {
                ntemp = ctemp->firstn;
                while(ntemp != NULL)
                {
                    if(findAsciiValueFromString(ntemp->name, one) == 1 )
                    {
                        control++;
                    }
                    if(findAsciiValueFromString(ntemp->name, two ) == 1)
                    {
                        control++;
                    }
                    if(findAsciiValueFromString(ntemp->name, three) == 1)
                    {
                        control++;
                    }
                    ntemp = ntemp->next;
                }
            }
            if(control == 3)
            {
                printf("{ [%d] [%s] [%s] } --> ",ctemp->plate, ctemp->name, ctemp->bolge);
                fprintf(fp,"{ [%d] [%s] [%s] } --> ",ctemp->plate, ctemp->name, ctemp->bolge);
                ListToNeighbours(ctemp,fp);
                newLine();
                fprintf(fp,"\n");
                isfind++;
            }
            ctemp = ctemp->next;
            control = 0;
        }
        if(isfind == 0)
        {
            printf("%d-%d aralığında ve %s'e ,%s'e ve %s'e komşu şehir",smallnum,bignum,one,two,three);
            fprintf(fp,"%d-%d aralığında ve %s'e ,%s'e ve %s'e komşu şehir",smallnum,bignum,one,two,three);
            printf(" koşullarına uygun bir şehir yoktur\n");
            fprintf(fp," koşullarına uygun bir şehir yoktur\n");
        }
    }

    if(key != 1 && key != 2 && key != 3)
    {
        printf("verilen sınırdan daha fazla komşu araştırmak istediniz , bu işlem sonlandırıldı ...");
        fprintf(fp,"verilen sınırdan daha fazla komşu araştırmak istediniz , bu işlem sonlandırıldı ...\n");
    }
    newLine();
    fprintf(fp,"\n");
}

/// şehirlerin tüm bilgilerini , ilk'ten sona gösteriyor
void ListToCity(FILE *fp)
{
    newLine();
    printf("Şehirler Listeleniyor");
    newLine();
    showStar(1,20);
    struct city *temp = first;
    if(temp == NULL)
    {
        printf("Listelenecek şehir yoktur !!! \n");
        fprintf(fp,"Listelenecek şehir yoktur !!! \n");
    }
    while(temp!=NULL)
    {
        printf("{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
        fprintf(fp,"{ [%d] [%s] [%s] [%d] } -->",temp->plate, temp->name, temp->bolge,temp->neighbourNum);
        ListToNeighbours(temp,fp);
        newLine();
        fprintf(fp,"\n");
        temp=temp->next;
    }

    showStar(1,20);
}
/// girilen şehrin komşuluklarını ilk'ten sona gösteriyor
void ListToNeighbours(struct city *listed, FILE *fp)
{

    struct neighbours *temp = listed->firstn;
    if(listed->neighbourNum == 0)
    {
        printf("[%s] şehrinin komşuları yoktur ",listed->name);
        fprintf(fp,"[%s] şehrinin komşuları yoktur ",listed->name);
    }
    while(temp!=NULL)
    {
        printf("[%d] [%s] -->",temp->plate,temp->name);
        fprintf(fp,"[%d] [%s] -->",temp->plate,temp->name);
        temp = temp->next;
    }
    fprintf(fp,"\n\n");
    newLine();
}
/// şehir adından şehir plakasını bulup int geri dönderiri
int findPlate(char *name)
{
    struct city *ctemp = first;
    struct neighbours *ntemp;
    while(ctemp != NULL)
    {
        if(findAsciiValueFromString(ctemp->name, name) == 1)
        {
            return ctemp->plate;
        }
        ntemp = ctemp->firstn;
        while(ntemp != NULL)
        {
            if(findAsciiValueFromString(ntemp->name, name) == 1)
            {
                return ntemp->plate;
            }
            ntemp = ntemp->next;
        }

        ctemp = ctemp->next;
    }
    return -1;
}

/// girilen şehir içinde istenilen komşuyu siler
int  deleteNeighbour(struct city *ctemp, int p)
{
    if(ctemp->neighbourNum != 0)
    {
        struct neighbours *deleted = ctemp->firstn;
        while(deleted != NULL)
        {
            if(deleted->plate == p)
            {
                break;
            }
            deleted = deleted->next;
        }
        if(deleted == NULL)
        {
            printf("Silinmesini istenen komşu bu şehirde yoktur !!! \n");
            return -1;
        }
        else
        {
            printf("Silinecek komşu : [%d] [%s]\n",deleted->plate,deleted->name);
            struct neighbours *lastn = ctemp->firstn;
            while(lastn->next != NULL)
            {
                lastn = lastn->next;
            }
            if(deleted->plate == ctemp->firstn->plate)
            {
                ctemp->firstn = ctemp->firstn->next;
                free(deleted);
                ctemp->neighbourNum--;

            }
            else if(deleted->plate == lastn->plate)
            {
                struct neighbours *ntemp = ctemp->firstn;
                while(ntemp->next != lastn)
                {
                    ntemp = ntemp->next;
                }
                lastn = ntemp;
                lastn->next = NULL;
                free(deleted);
                ctemp->neighbourNum--;

            }
            else
            {
                struct neighbours *ntemp = ctemp->firstn;
                while(ntemp->next != deleted)
                {
                    ntemp = ntemp->next;
                }
                ntemp->next = deleted->next;
                deleted->next = NULL;
                free(deleted);
                ctemp->neighbourNum--;
            }
        }

    }
    else
    {
        printf("şehrin komşusu yoktur !!!\n");
        return -2;
    }
    return 1;
}

/// istenilen şehirde , verilen plakaya göre komşuyu arar , eğerki aranan komşu o şehirde varsa , şehri geri dönderirir
struct city* findNeighbourFromPlate(struct city *ctemp, int p)
{
    struct neighbours *ntemp = ctemp->firstn;
    while(ntemp!= NULL)
    {
        if(ntemp->plate == p)
        {
            return ctemp;
        }
        ntemp = ntemp->next;
    }
    return NULL;
}

/// şehir silme metotu
int deleteCity(int p)
{
    if(first != NULL && last != NULL)
    {
        struct city * deleted = first;
        while(deleted != NULL)
        {
            if(deleted->plate == p)
            {
                break;
            }
            deleted = deleted->next;
        }
        if(deleted != NULL)
        {
            printf("Silinecek şehir : [%d] [%s] [%s] \n",deleted->plate,deleted->name,deleted->bolge);
            int dp = deleted->plate;
            if(dp == first->plate && dp == last->plate)
            {
                first = NULL;
                last = NULL;
                free(deleted);
            }
            else if(dp == first->plate)
            {
                first = first->next;
                first->before = NULL;
                deleted->next = NULL;
                free(deleted);
            }
            else if(dp == last->plate)
            {
                last = last->before;
                last->next = NULL;
                deleted->before = NULL;
                free(deleted);
            }
            else
            {
                deleted->before->next = deleted->next;
                deleted->next->before = deleted->before;
                deleted->before = NULL;
                deleted->next = NULL;
                free(deleted);
            }
            if(first != NULL && last != NULL)
            {
                printf("silinen şehir ile komşuluk ilişkisine sahip şehirlerden komşu olarak siliniyor \n");
                struct city *ctemp = first;
                int control = 0;
                while(ctemp != NULL)
                {
                    struct city * dn = findNeighbourFromPlate(ctemp, dp);
                    if(dn != NULL)
                    {
                        printf("[%d] [%s] [%s] : ",ctemp->plate,ctemp->name,ctemp->bolge);
                        deleteNeighbour(ctemp, dp);
                        control++;
                    }
                    ctemp=ctemp->next;
                }
                if(control == 0)
                {
                    printf("Silinecek komşuluk ilişkisi bulunamamıştır : \n");
                }

            }
        }
        else
        {
            printf("%d plakasına sahip şehir listede yoktur !!! \n",p);
            return -1;
        }
    }
    else
    {
        printf("Listede silinecek şehir kalmadı !!! \n");
        return -2;
    }
    newLine();
    return 1;

}

/// son komşunun isminin sonundaki '\n' i siler
void chomp(char *s)
{
    while(*s && *s != '\n' && *s != '\r')
        s++;

    *s = 0;
}

/// sehirler.txt ten şehirler yapısını okur
/// Hatalı metot
void readFromFile()
{
    ///şehir ekleme
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    FILE *fp = fopen("sehirler.txt", "r");
    if(fp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char *pch;
    int cp;
    char cname[17];
    char cregion[3];
    int count;
    while((read = getline(&line, &len, fp)) != -1)
    {
        count = 0;
        pch = strtok(line,",");
        while(pch != NULL)
        {
            if(count == 0)
            {
                cp = atoi(pch);
            }
            else if(count == 1)
            {
                strcpy(cname,pch);
            }
            else if(count == 2)
            {
                strcpy(cregion,pch);
            }
            count++;
            pch = strtok(NULL, ",");
        }
        addNewCity(cp,cname,cregion);

    }
    fclose(fp);

    ///komşu ekleme
    /// HATALI KISIM
    char *line2 = NULL;
    size_t len2 = 0;
    ssize_t read2 = 0;
    char nname[17];
    char tempname[17] = "";
    struct city *ctemp;
    FILE *fp2 = fopen("sehirler.txt", "r");
    if(fp2 == NULL)
    {
        exit(EXIT_FAILURE);
    }
    int np ;
    while((read2 = getline(&line2, &len2, fp2)) != -1)
    {
        np = -1;
        count =0;
        pch = strtok(line2,",");
        while(pch != NULL)
        {
            if(count == 0)
            {
                cp = atoi(pch);
            }
            else if(count == 1);
            else if(count == 2);
            else
            {
                strcpy(nname,pch);
                if(nname[strlen(nname)-1] == '\n')
                {
                    chomp(nname);
                }
                np = findPlate(nname);
                ctemp = findCityFromPlate(cp);
                addNeighbours(ctemp,np,nname);
            }
            count++;
            pch = strtok(NULL, ",");
        }
    }
    fclose(fp2);
}

///Başlangıç menüsü
void StartingMenu()
{
    PrintStar();
    printf("[0] \t Çıkış : \n");
    printf("[1] \t Arama : \n");
    printf("[2] \t Ekleme : \n");
    printf("[3] \t Silme : \n");
    PrintStar();
}

/// Arama bölümü  menüsüdür
void SearchingMenu()
{
    PrintStar();
    printf("[1] \t Şehir ismi ile arama : \n");
    printf("[2] \t Şehir plakası ile arama : \n");
    printf("[3] \t Bölgeye göre arama yaparsınız : \n");
    printf("[4] \t Sınırlandırılmış komşuluk sayısına göre arama : \n");
    printf("[5] \t Sınırlandırılmış komşuluk sayısı ve ortak komşu veya komşular ismine göre arama : \n");
    PrintStar();
}

/// Ekleme bölümü menüsüdür
void AddingMenu()
{
    PrintStar();
    printf("[1] \t Şehir ekle : \n");
    printf("[2] \t Komşu ekle : \n");
    PrintStar();
}

///Silme bölümü menüsüdür
void DeletingMenu()
{
    PrintStar();
    printf("[1] \t Komşu sil : \n");
    printf("[2] \t Şehir sil : \n");
    PrintStar();
}

///main metotu
int main()
{
    FILE *outputfp = fopen("cikti.txt", "w");
    if(outputfp == NULL)
    {
        printf("dosya açılamadı \n");
        exit(EXIT_FAILURE);
    }
    readFromFile();
    fprintf(outputfp,"Şehirler.txt'den şehirler listesi okunup oluşturulduktan sonra , şehirler listeleniyor : \n\n");
    ListToCity(outputfp);
    int isRunning = 1;
    while(isRunning != 0)
    {
        int key;
        StartingMenu();
        printf("Seçimi giriniz : ");
        scanf("%d",&key);
        printf("\n");

        if(key == 0)
        {
            newLine();
            showStar(1,30);
            printf("Program sonlandırıldı.\nKendinize iyi bakın.");
            fprintf(outputfp, "Program sonlandırıldı.\nKendinize iyi bakın.");
            newLine();
            showStar(1,30);
            isRunning = 0;
        }
        else if(key == 1)
        {
            PrintSharp();
            printf("Arama işlemi bölümündesiniz\n");
            fprintf(outputfp,"[1]Arama işlemi bölümündesiniz : \n");
            PrintSharp();

            int searchingKey;
            SearchingMenu();
            printf("Seçimi giriniz : ");
            scanf("%d",&searchingKey);
            if(searchingKey == 1)
            {
                char cityName[17];
                fprintf(outputfp, "[1.1]İsimden şehir arama bölümündesiniz : \n");
                printf("Bir şehir adı giriniz : ");
                scanf("%s",&cityName);
                fprintf(outputfp,"Araştırılmasını istediğiniz şehir adı : %s\n",cityName);
                struct city *searched = findCityFromName(cityName);
                if(searched != NULL)
                {
                    newLine();
                    printf("Aradığınız şehir : \n");
                    fprintf(outputfp, "Aradığınız şehir : \n ");
                    PrintStar();
                    printf("{ [%d] [%s] [%s] [%d] } -->",searched->plate,searched->name,searched->bolge,searched->neighbourNum);
                    fprintf(outputfp,"{ [%d] [%s] [%s] [%d] } -->",searched->plate,searched->name,searched->bolge,searched->neighbourNum);
                    ListToNeighbours(searched,outputfp);
                    PrintStar();
                    newLine();
                    fprintf(outputfp,"\n");
                }
                else
                {
                    newLine();
                    showStar(1,30);
                    printf("%s ismiyle aradığınız şehir listede yoktur !!! ",cityName);
                    fprintf(outputfp,"%s ismiyle aradığınız şehir listede yoktur !!! ",cityName);
                    newLine();
                    fprintf(outputfp,"\n\n");
                    showStar(1,30);
                }
            }
            else if(searchingKey == 2)
            {
                int cityPlate;
                fprintf(outputfp,"[1.2]Plakadan şehir arama bölümündesiniz : \n");
                printf("Bir şehir plakası giriniz : ");
                scanf("%d",&cityPlate);
                fprintf(outputfp,"araştırılmasını istediğiniz şehir plakası : %d\n",cityPlate);
                struct city *searched = findCityFromPlate(cityPlate);
                if(searched != NULL)
                {
                    newLine();
                    printf("Aradığınız şehir : \n");
                    fprintf(outputfp,"\nAradığınız şehir : \n");
                    PrintStar();
                    printf("{ [%d] [%s] [%s] [%d] } -->",searched->plate,searched->name,searched->bolge,searched->neighbourNum);
                    fprintf(outputfp, "{ [%d] [%s] [%s] [%d] } -->",searched->plate,searched->name,searched->bolge,searched->neighbourNum);
                    ListToNeighbours(searched,outputfp);
                    PrintStar();
                    newLine();
                    fprintf(outputfp,"\n");
                }
                else
                {
                    newLine();
                    PrintStar();
                    printf("%d plakasıyla aradığınız şehir listede yoktur !!! \n",cityPlate);
                    fprintf(outputfp,"\n%d plakasıyla aradığınız şehir listede yoktur !!! \n",cityPlate);
                    PrintStar();
                    newLine();
                    fprintf(outputfp,"\n");
                }
            }
            else if(searchingKey == 3)
            {
                char cityRegion[3];
                fprintf(outputfp,"[1.3]Aynı bölgedeki şehirleri arama bölümündesiniz\n");
                fprintf(outputfp,"Aranabilecek bölgeler (MA , AK , KA , DA , EG , GA , IA)\n");
                printf("Bölgeler : (MA , AK , KA , DA , EG , GA ,IA)\n");
                printf("Bir bölge adı giriniz : ");
                scanf("%s",&cityRegion);
                ListCityInSameRegion(cityRegion, outputfp);
            }
            else if(searchingKey == 4)
            {
                fprintf(outputfp,"[1.4]Sınırlandırılmış komşu sayısına göre şehir arama bölümündesiniz\n");
                ListCityToNeighboursNumber(outputfp);
            }
            else if(searchingKey == 5)
            {
                fprintf(outputfp,"[1.5]Sınırlandırılmış komşu sayısı ve ortak komşulara göre şehir arama bölümündesiniz\n");
                ListCityHaveSameNeighbours(outputfp);
            }
            else
            {
                printf("Yanlış bir arama işlemi seçimi yaptınız , tüm program baştan başlıyor ... \n");
                fprintf(outputfp, "Arama bölümünde yanlış bir seçim yaptınız \n Program baştan başlatılıyor!!!\n\n");
            }

        }
        else if(key == 2)
        {
            int addingKey;
            printf("Ekleme işlemi bölümündesiniz : \n");
            fprintf(outputfp,"[2]Ekleme işlemi bölümündesiniz : \n");
            AddingMenu();
            printf("Seçimi giriniz : ");
            scanf("%d",&addingKey);
            if(addingKey == 1)
            {
                fprintf(outputfp,"[2.1]Şehir ekleme bölümündesiniz : \n");
                char cityName[17];
                char cityRegion[3];
                int cityPlate;
                printf("yeni eklemek istediğiniz şehrin bilgilerini giriniz : \n");
                printf("ismi : ");
                scanf("%s",&cityName);
                printf("plakası : ");
                scanf("%d",&cityPlate);
                printf("bölgesi : ");
                scanf("%s",&cityRegion);
                newLine();
                showStar(1,20);
                printf("şehir oluşturuluyor ...\n");
                fprintf(outputfp,"[%d] [%s] [%s] şehri oluşturuluyor ...\n",cityPlate,cityName,cityRegion);
                int test = addNewCity(cityPlate, cityName, cityRegion);
                if(test == -1)
                {
                    fprintf(outputfp,"oluşturmaya çalıştığınız şehirle aynı isimde bir şehir vardır , yapıda :\n ");
                    fprintf(outputfp, "şehir oluşturulamıyor !!! \n");
                    fprintf(outputfp,"\n");
                    continue;
                }
                if(test == -2)
                {
                    fprintf(outputfp,"oluşturmaya çalıştığınız şehirle aynı plakada bir şehir vardır , yapıda :\n ");
                    fprintf(outputfp, "şehir oluşturulamıyor !!! \n");
                    fprintf(outputfp,"\n");
                    continue;
                }
                if(test == -3)
                {
                    fprintf(outputfp,"oluşturmaya çalıştığınız şehirle aynı isimde ve aynı plakada bir şehir vardır , yapıda :\n ");
                    fprintf(outputfp, "şehir oluşturulamıyor !!! \n");
                    fprintf(outputfp,"\n");
                    continue;
                }
                newLine();
                showStar(1,20);
                int wantSee;
                printf("eklediğiniz şehri listede görmek ister misiniz : evet (1) \t hayır (0) ");
                scanf("%d",&wantSee);
                if(wantSee == 1)
                {
                    fprintf(outputfp,"Yeni şehir eklendikten sonra tüm şehirler sıralanıyor : \n");
                    ListToCity(outputfp);
                }
            }
            else if(addingKey == 2)
            {
                fprintf(outputfp,"[2.2]Komşu ekleme bölümündesiniz : \n");
                struct city *added;
                int which;
                printf("yeni komşu eklemek istediğiniz şehrin plakasını(1) mı girmek istersiniz \nyoksa şehrin ismini(2) mi ? : \n seçiminiz : ");
                scanf("%d",&which);
                int addedPlate;
                char addedName[17];
                if(which == 1)
                {
                    printf("Şehrin plaka giriniz : ");
                    scanf("%d",&addedPlate);
                    added = findCityFromPlate(addedPlate);
                }
                else if(which == 2)
                {
                    printf("Şehrin adını giriniz : ");
                    scanf("%s",&addedName);
                    added = findCityFromName(addedName);
                }
                else
                {
                    printf("yanlış işlem seçimi program baştan başlatılıyor ... \n");
                    continue;
                }
                struct city *ctemp = first;
                int Platecontrol = -1;
                int Namecontrol = -1;
                int neighbourPlate;
                char neighbourName[15];
                printf("eklemek istediğiniz komşunun bilgilerini giriniz : \n");
                printf("plaka : ");
                scanf("%d",&neighbourPlate);
                while(ctemp != NULL)
                {
                    if(ctemp->plate == neighbourPlate)
                    {
                        Platecontrol = 1;
                        break;
                    }
                    ctemp = ctemp->next;
                }
                if(Platecontrol == 1)
                {
                    printf("girdiğiniz plakayla eşleşen şehir vardır , ekleyeceğiniz komşunun ismi , eşleşen şehrin isminden alınıyor ...\n");
                    strcpy(neighbourName, ctemp->name);
                }
                else
                {
                    printf("isim : ");
                    scanf("%s",&neighbourName);
                    ctemp = first;
                    while(ctemp != NULL)
                    {
                        if(findAsciiValueFromString(neighbourName, ctemp->name) == 1)
                        {
                            Namecontrol = 1;
                            break;
                        }
                        ctemp = ctemp->next;
                    }
                }
                if(Namecontrol == 1)
                {
                    printf("girdiniz isimle eşleşen şehir bulunmuştur ,ekleyeceğiniz komşunun plakası , eşleşen şehrin plakası alınıyor ...\n");
                    neighbourPlate = ctemp->plate;
                }
                if(neighbourPlate == added->plate || findAsciiValueFromString(added->name, neighbourName) == 1)
                {
                    PrintSharp();
                    printf("Eklemek istediğiniz komşuluk şehrin kendisidir , böyle bir komşuluk mümkün değildir !!!\n");
                    fprintf(outputfp,"Eklemek istediğiniz komşuluk şehrin kendisidir , böyle bir komşuluk mümkün değildir !!!\n");
                    printf("Ekleme işleminiz iptal edilmiştir !!!\n");
                    fprintf(outputfp,"Ekleme işleminiz iptal edilmiştir !!!\n\n");
                    PrintSharp();
                    continue;
                }

                newLine();
                showStar(1,30);
                printf("istediğiniz şehre istediğiniz komşu ekleniyor : ... \n");
                fprintf(outputfp,"[%d] [%s] şehrine istediğiniz [%d] [%s] ekleniyor : ... \n",added->plate,added->name,neighbourPlate,neighbourName);
                int control = addNeighbours(added,neighbourPlate,neighbourName);
                if(control == -1)
                {
                    fprintf(outputfp,"Eklemek istediğiniz komşulukla aynı isimde komşuluk vardır : \n");
                    fprintf(outputfp, "Komşuluk eklenemiyor !!! \n");
                    fprintf(outputfp,"\n");
                    continue;
                }
                else if(control == -2)
                {
                    fprintf(outputfp, "Eklemek istediğiniz komşulukla aynı plakadan komşuluk vardır : \n");
                    fprintf(outputfp, "Komşuluk eklenemiyor !!! \n");
                    fprintf(outputfp, "\n");
                    continue;
                }
                else if(control == -3)
                {
                    fprintf(outputfp,"Eklemek istediğiniz komşulukla aynı plakadan ve aynı isimden komşuluk vardır : \n");
                    fprintf(outputfp, "Komşuluk eklenemiyor !!! \n");
                    fprintf(outputfp,"\n");
                    continue;
                }
                newLine();
                showStar(1,30);
                newLine();
                int wantSee = 0;
                printf("komşuluk eklediğiniz şehrin güncel bilgilerini görmek ister misiniz : \n(evet[1] , hayır[2] )\n seçiminiz : ");
                scanf("%d",&wantSee);
                if(wantSee == 1)
                {
                    fprintf(outputfp,"Yeni komşu eklendikten sonra komşu eklenen şehir gösteriliyor : \n");
                    printf("{ [%d] [%s] [%s] [%d] } -->",added->plate,added->name,added->bolge,added->neighbourNum);
                    fprintf(outputfp,"{ [%d] [%s] [%s] [%d] } -->",added->plate,added->name,added->bolge,added->neighbourNum);
                    ListToNeighbours(added,outputfp);

                }

            }
            else
            {
                printf("Yanlış bir ekleme işlemi seçtiniz !!! : \n Tüm program baştan başlatılıyor ...\n");
                fprintf(outputfp,"Yanlış bir ekleme işlemi seçtiniz !!! : \n Tüm program baştan başlatılıyor ...\n\n");
            }
        }
        else if(key == 3)
        {
            fprintf(outputfp,"[3]Silme işlemleri bölümündesiniz : \n");
            int deletingKey;
            printf("Silme işlemi bölümündesiniz : \n");
            DeletingMenu();
            printf("Seçimi giriniz : ");
            scanf("%d",&deletingKey);
            if(deletingKey == 1)
            {
                int choose;

                newLine();
                printf("Komşu silme \n");
                fprintf(outputfp,"[3.1]Komşu silme bölümündesiniz : \n");
                PrintStar();
                printf("[1] Plaka ile \n");
                printf("[2] İsim  ile \n");
                PrintStar();

                printf("Seçimi giriniz : ");
                scanf("%d",&choose);

                if(choose == 1)
                {
                    fprintf(outputfp,"[3.1.1]Komşuluğu plaka'dan seçerek silme işlemi bölümündesiniz : \n");
                    int cityPlate;
                    printf("Şehir plakası giriniz : ");
                    scanf("%d",&cityPlate);
                    fflush(stdin);
                    struct city *deleted = findCityFromPlate(cityPlate);
                    if(deleted == NULL)
                    {
                        printf("%d plakasına sahip şehir listede yoktur !!! \n Komşuluk silme işlemi yapılamıyor !!! \n Program baştan başlatılıyor : \n",cityPlate);
                        fprintf(outputfp,"%d plakasına sahip şehir listede yoktur !!! \n Komşuluk silme işlemi yapılamıyor !!! \n Program baştan başlatılıyor : \n\n",cityPlate);
                        continue;
                    }
                    int neighbourPlate;
                    printf("Silmek istediğini komşuluğun plakasını giriniz : ");
                    scanf("%d",&neighbourPlate);
                    newLine();
                    showStar(1,30);
                    printf("istediğiniz komşuluk siliniyor : ... \n");
                    fprintf(outputfp,"[%d] plakalı komşuluk siliniyor : ... \n",neighbourPlate);
                    int test = deleteNeighbour(deleted, neighbourPlate);
                    if(test == -1)
                    {
                        fprintf(outputfp,"[%d] [%s] 'in %d plakasına sahip komşusu yoktur : \n\n",deleted->plate,deleted->name,neighbourPlate);
                        continue;
                    }
                    if(test == -2)
                    {
                        fprintf(outputfp, "[%d] [%s] 'nin komşusu yoktur : \n\n",deleted->plate,deleted->name);
                        continue;
                    }
                    newLine();
                    showStar(1,30);
                    newLine();
                    int wantSee;
                    printf("Komşuluğu silinen şehrin güncel bilgilerini görmek ister misiniz : \n(evet[1] , Hayır[2]) \n seçiminiz : ");
                    scanf("%d",&wantSee);
                    if(wantSee == 1)
                    {
                        fprintf(outputfp,"Komşuluk silme işleminden sonra komşuluğu silinen şehir gösteriliyor : \n");
                        printf("{ [%d] [%s] [%s] [%d] } -->",deleted->plate,deleted->name,deleted->bolge,deleted->neighbourNum);
                        fprintf(outputfp,"{ [%d] [%s] [%s] [%d] } -->",deleted->plate,deleted->name,deleted->bolge,deleted->neighbourNum);
                        ListToNeighbours(deleted,outputfp);
                    }
                }
                else if(choose == 2)
                {
                    fprintf(outputfp,"[3.1.2]Komşuluğu isim'den seçerek silme işlemi bölümündesiniz : \n");
                    char cityName[17];
                    printf("Şehir ismi giriniz : ");
                    scanf("%s",&cityName);
                    struct city *deleted = findCityFromName(cityName);
                    if(deleted == NULL)
                    {
                        printf("%s ismine sahip şehir listede yoktur !!! \n Komşuluk silme işlemi yapılamıyor !!! \n Program baştan başlatılıyor : \n",cityName);
                        fprintf(outputfp,"%s ismine sahip şehir listede yoktur !!! \n Komşuluk silme işlemi yapılamıyor !!! \n Program baştan başlatılıyor : \n\n",cityName);
                        continue;
                    }
                    char neighbourName[17];
                    printf("silmek istediğiniz komşuluğun ismini giriniz : ");
                    scanf("%s",&neighbourName);
                    int neighbourPlate = findPlate(neighbourName);
                    newLine();
                    showStar(1,30);
                    printf("istediğiniz komşuluk siliniyor : ... \n");
                    fprintf(outputfp,"[%d ] [%s] komşuluğu siliniyor : ... \n",neighbourPlate,neighbourName);
                    int test = deleteNeighbour(deleted, neighbourPlate);
                    if(test == -1)
                    {
                        fprintf(outputfp,"[%d] [%s] 'in %s ismine sahip komşusu yoktur : \n\n",deleted->plate,deleted->name,neighbourName);
                        continue;
                    }
                    if(test == -2)
                    {
                        fprintf(outputfp, "[%d] [%s] 'nin silinebilecek komşusu yoktur : \n\n",deleted->plate,deleted->name);
                        continue;
                    }
                    newLine();
                    showStar(1,30);
                    newLine();
                    int wantSee;
                    printf("Komşuluğu silinen şehrin güncel bilgilerini görmek ister misiniz : \n(evet[1] , Hayır[2]) \n seçiminiz : ");
                    scanf("%d",&wantSee);
                    if(wantSee == 1)
                    {
                        fprintf(outputfp,"Komşuluk silme işleminden sonra komşuluğu silinen şehir gösteriliyor : \n");
                        printf("{ [%d] [%s] [%s] [%d] } -->",deleted->plate,deleted->name,deleted->bolge,deleted->neighbourNum);
                        fprintf(outputfp,"{ [%d] [%s] [%s] [%d] } -->",deleted->plate,deleted->name,deleted->bolge,deleted->neighbourNum);
                        ListToNeighbours(deleted,outputfp);
                    }
                }
            }
            else if(deletingKey == 2)
            {
                int choose;
                fprintf(outputfp,"[3.2]Şehir silme bölümündesiniz : \n");
                newLine();
                printf("Şehir silme \n");
                PrintStar();
                printf("[1] Plaka ile \n");
                printf("[2] İsim  ile \n");
                PrintStar();

                printf("Seçimi giriniz : ");
                scanf("%d",&choose);

                if(choose == 1)
                {
                    fprintf(outputfp,"[3.2.1]Şehri plaka'dan seçerek silme bölümündesiniz : \n");
                    int cityPlate;
                    printf("Plaka giriniz : ");
                    scanf("%d",&cityPlate);
                    newLine();
                    showStar(1,30);
                    printf("Şehir siliniyor...\n");
                    fprintf(outputfp,"[%d] plakalı şehir siliniyor...\n",cityPlate);
                    int test = deleteCity(cityPlate);
                    if(test == -1)
                    {
                        fprintf(outputfp, "%d plakasına sahip şehir , listede yoktur !!! \n\n",cityPlate);
                        continue;
                    }
                    if(test == -2)
                    {
                        fprintf(outputfp, "Listede şehir yoktur !!! \n\n");
                        continue;
                    }
                    printf("Şehir silindi.\n");
                    fprintf(outputfp,"[%d] plakalı şehir silindi.\n",cityPlate);
                    newLine();
                    showStar(1,30);
                    int wantSee;
                    printf("Silme işleminden sonra güncel listeyi görmek ister misiniz : \n (evet[1] , hayır[2]) \n şeçiminiz : ");
                    scanf("%d",&wantSee);
                    if(wantSee == 1)
                    {
                        fprintf(outputfp,"Şehir silme işleminden sonra şehirler listesi gösteriliyor : \n\n");
                        ListToCity(outputfp);
                    }
                }
                else if(choose == 2)
                {
                    fprintf(outputfp,"[3.2.2]Şehri isim'den seçerek silme bölümündesiniz : \n");
                    char cityName[17];
                    printf("Şehir ismini giriniz : ");
                    scanf("%s",&cityName);
                    int cityPlate = findPlate(cityName);
                    newLine();
                    showStar(1,30);
                    printf("istediğiniz şehir siliniyor : ...\n");
                    fprintf(outputfp,"[%d] [%s] şehri siliniyor : ...\n",cityPlate,cityName);
                    int test = deleteCity(cityPlate);
                    if(test == -1)
                    {
                        fprintf(outputfp, "%s ismine sahip şehir , listede yoktur !!! \n\n",cityName);
                        continue;
                    }
                    if(test == -2)
                    {
                        fprintf(outputfp, "Listede şehir yoktur !!! \n\n");
                        continue;
                    }
                    newLine();
                    showStar(1,30);
                    int wantSee;
                    printf("Silme işleminden sonra güncel listeyi görmek ister misiniz : \n (evet[1] , hayır[2]) \n şeçiminiz : ");
                    scanf("%d",&wantSee);
                    if(wantSee == 1)
                    {
                        fprintf(outputfp,"Şehir silme işleminden sonra şehirler listesi gösteriliyor : \n\n");
                        ListToCity(outputfp);
                    }
                }
            }
            else
            {
                printf("Yanlış bir silme işlemi seçtiniz !!!  : \n Tüm program baştan başlatılıyor ...\n");
                fprintf(outputfp,"Yanlış bir silme işlemi seçtiniz !!!  : \n Tüm program baştan başlatılıyor ...\n\n");
            }
        }
    }
    fprintf(outputfp,"\n\nTüm işlemler bittikten sonra şehirler listesi listeleniyor : \n\n");
    ListToCity(outputfp);
    fclose(outputfp);
    return 0;
}

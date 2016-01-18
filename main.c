#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>
#define SYLLABLE_MAX_LENGH 16
#define RHYME_WITH_TONE_MAX_LENGH 5
#define PHONE_RHYME_MAX_LENGH 12
#define PHONE_CONSONANT_MAX_LENGH 4
#define RHYME_MAX_LENGH 5
#define CONSONANT_MAX_LENGH 4
#define PHONE_RHYME_WITH_TONE_MAX_LENGH 13
#define PHONE_SYLLABEL_MAX_LENGH PHONE_CONSONANT_MAX_LENGH+PHONE_RHYME_WITH_TONE_MAX_LENGH-1
#define SYLLABLE_MAX_SIZE 30000
#define RHYME_WITH_TONE_MAX_SIZE 1000
#define RHYME_MAX_SIZE 300
#define CONSONANT_MAX_SIZE 100

void print_result_festival_to_file(void (*split_tone)(wchar_t *,wchar_t *),char * filename);
void split_tone_xampa(wchar_t *string, wchar_t * result);
void replace_one_char(wchar_t *string, wchar_t symbol_be_replace, wchar_t symbol_replace);
void insert_tone_and_join(wchar_t *rhyme,wchar_t *phone_rhyme);
void insert_tone_and_join_end_ptk(int(*find_index_to_insert_tone_func)(wchar_t*),void(*add_element_func)(wchar_t,wchar_t*,wchar_t*),wchar_t *rhyme,wchar_t *phone_rhyme);
void insert_tone_and_join_non_end_ptk(int(*find_index_to_insert_tone_func)(wchar_t*),void(*add_element_func)(wchar_t,wchar_t*,wchar_t*),wchar_t *rhyme,wchar_t *phone_rhyme);
int  is_end_ptk(wchar_t *rhyme);
int find_index_to_insert_tone(wchar_t *rhyme);
int find_index_to_insert_tone_for_q(wchar_t *rhyme);
void add_element(wchar_t first_of_rhyme_non_tone,wchar_t *rhyme,wchar_t *phone_rhyme);
void add_element_for_q(wchar_t first_of_rhyme_non_tone,wchar_t *rhyme,wchar_t *phone_rhyme);
void add_element_for_non_tone(wchar_t first_of_rhyme_non_tone,wchar_t *rhyme,wchar_t *phone_rhyme);
void print_array_of_string(int size,int len,wchar_t arrString[size][len]);
void insert_tone_and_join_for_all_element();

void join_one_time(wchar_t *consonant, wchar_t *rhyme,wchar_t *phone_consonant, wchar_t *phone_rhyme);
void join_one_time_with_rule(wchar_t *consonant, wchar_t first_of_rhyme_non_tone,  wchar_t *rhyme,wchar_t *phone_consonant, wchar_t *phone_rhyme);
void join_consonant_rhyme(wchar_t first_of_rhyme_non_tone,  wchar_t *rhyme, wchar_t *phone_rhyme);
void add_exeception_of_literal_y();

void store_consonants_rhymes();
void read_file(FILE *f, int max_size, int max_len1,int max_len2,wchar_t string1[max_size][max_len1], wchar_t string2[max_size][max_len2],int *count);
wchar_t * split(wchar_t *buff[], wchar_t symbol);
void remove_q_from_initial_consonants();
void printf_result_to_file(char *filename);
wchar_t syllables[SYLLABLE_MAX_SIZE][SYLLABLE_MAX_LENGH];
wchar_t phone_syllables[SYLLABLE_MAX_SIZE][PHONE_SYLLABEL_MAX_LENGH];
wchar_t rhymes_with_tone[RHYME_WITH_TONE_MAX_SIZE][RHYME_WITH_TONE_MAX_LENGH];
wchar_t phone_rhymes[RHYME_MAX_SIZE][PHONE_RHYME_MAX_LENGH];

wchar_t phone_consonants[CONSONANT_MAX_SIZE][PHONE_CONSONANT_MAX_LENGH];
wchar_t consonants[CONSONANT_MAX_SIZE][CONSONANT_MAX_LENGH];
wchar_t rhymes[RHYME_MAX_SIZE][RHYME_MAX_LENGH];
wchar_t q[4], phone_q[4];
int rhyme_size=0;
int consonant_size=0;
int rhymes_with_tone_size=0;
int syllables_size=0;
int main(){
    setlocale(LC_CTYPE, "");
    store_consonants_rhymes();
    remove_q_from_initial_consonants();
    insert_tone_and_join_for_all_element();
    printf_result_to_file("result_xampa");
    print_result_festival_to_file(split_tone_xampa,"result_festival");
//    print_array_of_string(syllables_size,SYLLABLE_MAX_LENGH,syllables);
//    print_array_of_string(syllables_size,PHONE_SYLLABEL_MAX_LENGH,phone_syllables);
}
void remove_q_from_initial_consonants(){
	int i;
	for(i=0;wcscmp(consonants[i],L"q")!=0;i++);
	wcscpy(q,consonants[i]);
	wcscpy(phone_q,phone_consonants[i]);
	while(i<consonant_size){
		wcscpy(consonants[i],consonants[i+1]);
		i++;
	}
	consonant_size--;
}
wchar_t * split(wchar_t *buff[], wchar_t symbol){
  wchar_t *temp = (wchar_t *)malloc(1000*sizeof(wchar_t));
  int i=0;
  while(**buff!=symbol&&**buff!='\0'){
    temp[i]=**buff;
    *buff=*buff+1;
    i++;
  }
  (*buff)++;
  temp[i]='\0';
  return temp;
}

void read_file(FILE *f, int max_size, int max_len1,int max_len2,wchar_t string1[max_size][max_len1], wchar_t string2[max_size][max_len2],int *count){
		wchar_t *string=(wchar_t*)malloc(256*sizeof(wchar_t)),*buff,c;
		while(fgetws(string,256,f)!=NULL){
    		string[wcslen(string)-1]='\0';
    		wcscpy(string1[*count],split(&string,L' '));
    		wcscpy(string2[*count],string);
    		(*count)++;
		}
}

void store_consonants_rhymes(){
	FILE *initial_consonant = fopen("initial_consonants","r");
	FILE *rhyme_phone = fopen("rhymes_phones","r");
    read_file(initial_consonant,CONSONANT_MAX_SIZE,CONSONANT_MAX_LENGH,PHONE_CONSONANT_MAX_LENGH,consonants,phone_consonants,&consonant_size);
    read_file(rhyme_phone,RHYME_MAX_SIZE,RHYME_MAX_LENGH,PHONE_RHYME_MAX_LENGH,rhymes,phone_rhymes,&rhyme_size);
    fclose(initial_consonant);
    fclose(rhyme_phone);
}
void add_exeception_of_literal_y(){
	int i;
	wchar_t temp_rhyme[RHYME_MAX_LENGH];
	for(i=0;i<rhyme_size;i++){
		if(wcscmp(rhymes[i],L"iêc")==0||
		   wcscmp(rhymes[i],L"iêng")==0||wcscmp(rhymes[i],L"iêt")==0||
		   wcscmp(rhymes[i],L"iên")==0||wcscmp(rhymes[i],L"iêp")==0||
		   wcscmp(rhymes[i],L"iêm")==0||wcscmp(rhymes[i],L"iêu")==0){
		   wcscpy(temp_rhyme,rhymes[i]);
		   temp_rhyme[0]=L'y';
		   if(is_end_ptk(temp_rhyme))
			   insert_tone_and_join_end_ptk(find_index_to_insert_tone,add_element_for_non_tone,temp_rhyme,phone_rhymes[i]);
		   else{
			   wcscat(phone_rhymes[i],L"-A1");
			   add_element_for_non_tone(temp_rhyme[0],temp_rhyme,phone_rhymes[i]);
			   insert_tone_and_join_non_end_ptk(find_index_to_insert_tone,add_element_for_non_tone,temp_rhyme,phone_rhymes[i]);
		   }
		}
	}
}
void join_one_time(wchar_t *consonant, wchar_t *rhyme,wchar_t *phone_consonant, wchar_t *phone_rhyme){
    wcscpy(syllables[syllables_size],consonant);
    wcscat(syllables[syllables_size],rhyme);
    wcpcpy(phone_syllables[syllables_size],phone_consonant);
    wcscat(phone_syllables[syllables_size],L"-");
    wcscat(phone_syllables[syllables_size],phone_rhyme);
    syllables_size++;
}
void join_one_time_with_rule(wchar_t *consonant, wchar_t first_of_rhyme_non_tone,  wchar_t *rhyme,wchar_t *phone_consonant, wchar_t *phone_rhyme){
    int len;
    if(wcscmp(consonant,L"gi")==0){
        len = wcslen(rhyme);
        if(first_of_rhyme_non_tone==L'i'){
            if(!(len ==2 &&(rhyme[1]==L'u'||rhyme[1]==L'a'))){
            	if(first_of_rhyme_non_tone==L'ê')
            		join_one_time(L"g",rhyme,L"z-i",phone_rhyme);
            	else if(first_of_rhyme_non_tone==L'i' && len==1)
            		join_one_time(L"g",rhyme,L"z",phone_rhyme);
            	else
            		join_one_time(L"g",rhyme,phone_consonant,phone_rhyme);
            }
        }else{
            if(!(len==2&&first_of_rhyme_non_tone==L'ê'))
                join_one_time(consonant,rhyme,phone_consonant,phone_rhyme);
        }
    }
    else{
        if(wcscmp(consonant,L"g")==0){
            if(first_of_rhyme_non_tone!=L'i')
                join_one_time(consonant,rhyme,phone_consonant,phone_rhyme);
        }else{
        	if(wcscmp(consonant,L"c")!=0||(first_of_rhyme_non_tone!=L'i'&&first_of_rhyme_non_tone!=L'e'&&first_of_rhyme_non_tone!=L'ê'&&first_of_rhyme_non_tone!=L'y'))
        		join_one_time(consonant,rhyme,phone_consonant,phone_rhyme);
        }
    }
}
// void join_one_time_with_rule(wchar_t *consonant, wchar_t first_of_rhyme_non_tone,  wchar_t *rhyme){
//     if(wcscmp(consonant,L"q")==0){
//         if(first_of_rhyme_non_tone==L'u'&&wcslen(rhyme)>1){
//             join_one_time(consonant,rhyme);
//         }
//     }else{
//         if(wcscmp(consonant,L"gi")==0){
//             if(first_of_rhyme_non_tone!=L'i'&&first_of_rhyme_non_tone!=L'y'){
//                 join_one_time(consonant,rhyme);
//             }
//         }else{
//             if(first_of_rhyme_non_tone==L'i'||first_of_rhyme_non_tone==L'e'||first_of_rhyme_non_tone==L'ê'||first_of_rhyme_non_tone==L'y'){
//                 if(wcscmp(consonant,L"c")!=0&&wcscmp(consonant,L"g")!=0&&wcscmp(consonant,L"ng")!=0)
//                     join_one_time(consonant,rhyme);
//             }else if(wcscmp(consonant,L"k")!=0&&wcscmp(consonant,L"gh")!=0&&wcscmp(consonant,L"ngh")!=0&&!(wcscmp(consonant,L"c")==0&&
//                     (rhyme[1]==L'y'||rhyme[1]==L'ý'||rhyme[1]==L'ỳ'||rhyme[1]==L'ỵ'||rhyme[1]==L'ỹ'||rhyme[1]==L'ỷ'||first_of_rhyme_non_tone==L'y')))
//                         join_one_time(consonant,rhyme);
//         }
//     }
// }
void join_consonant_rhyme(wchar_t first_of_rhyme_non_tone,  wchar_t *rhyme, wchar_t *phone_rhyme){
    int i;
    for(i=0;i<consonant_size;i++){
        join_one_time_with_rule(consonants[i],first_of_rhyme_non_tone,rhyme,phone_consonants[i],phone_rhyme);
    }
}
void insert_tone_and_join_for_all_element(){
    int i;
    for(i=0;i<rhyme_size;i++)
        insert_tone_and_join(rhymes[i],phone_rhymes[i]);
    add_exeception_of_literal_y();
}
void print_array_of_string(int size,int len,wchar_t arrString[size][len]){
    int i;
    for(i=0;i<size;i++){
        printf("%ls\n",arrString[i]);
    }
}
void printf_result_to_file(char *filename){
	int i;
	FILE *f = fopen(filename,"w");
	for(i=0;i<syllables_size;i++){
		fwprintf(f,L"%ls %ls\n",syllables[i],phone_syllables[i]);
	}
	fclose(f);
}
void replace_one_char(wchar_t *string, wchar_t symbol_be_replace, wchar_t symbol_replace){
	while(*string!=L'\0'){
		if(*string==symbol_be_replace)
			*string = symbol_replace;
		string++;
	}
}
void split_tone_xampa(wchar_t *string, wchar_t * result){
	int len = wcslen(string);
	result[0] = string[len-2];
	result[1] = string[len-1];
	result[2] = L'\0';
	string[len-3]=L'\0'; // len-3 la vi tri dau cach cuoi cung
}
void print_result_festival_to_file(void (*split_tone)(wchar_t *,wchar_t *), char *filename){
	int i;
	wchar_t tone[3],temp[PHONE_SYLLABEL_MAX_LENGH];
	FILE *f=fopen(filename,"w");
	for(i=0;i<syllables_size;i++){
//		printf("%d+%ls\n",wcslen(phone_syllables[i]),phone_syllables[i]);
//		wcscpy(temp,phone_syllables[i]);
		replace_one_char(phone_syllables[i],L'-',L' ');
		split_tone(phone_syllables[i],tone);
		fwprintf(f,L"(\"%ls\" nil ((( %ls ) %ls)) )\n",syllables[i],phone_syllables[i],tone);
	}
	fclose(f);
}
void add_element(wchar_t first_of_rhyme_non_tone,wchar_t *rhyme,wchar_t *phone_rhyme){
    int len = wcslen(rhyme);
	if(first_of_rhyme_non_tone!=L'i'||(len<2||rhyme[3]==L'h')){
		wcscpy(syllables[syllables_size],rhyme);
		wcpcpy(phone_syllables[syllables_size],phone_rhyme);
		syllables_size++;
    }
    join_consonant_rhyme(first_of_rhyme_non_tone,rhyme,phone_rhyme);
}
void add_element_for_q(wchar_t first_of_rhyme_non_tone,wchar_t *rhyme,wchar_t *phone_rhyme){
		join_one_time(q,rhyme,phone_q,phone_rhyme);
}
void add_element_for_non_tone(wchar_t first_of_rhyme_non_tone,wchar_t *rhyme,wchar_t *phone_rhyme){
		join_one_time(L"",rhyme,L"",phone_rhyme);
}

void insert_tone_and_join(wchar_t *rhyme,wchar_t *phone_rhyme){
    if(is_end_ptk(rhyme)){
        insert_tone_and_join_end_ptk(find_index_to_insert_tone,add_element,rhyme,phone_rhyme);
        if(rhyme[0]==L'u'&&wcslen(rhyme)>1&&rhyme[1]!=L'n'&&rhyme[1]!=L't'&&rhyme[1]!=L'p'&&rhyme[1]!=L'm')
        	insert_tone_and_join_end_ptk(find_index_to_insert_tone_for_q,add_element_for_q,rhyme,phone_rhyme);
    }else{
    	wcscat(phone_rhyme,L"-A1");
    	add_element(rhyme[0],rhyme,phone_rhyme);
    	insert_tone_and_join_non_end_ptk(find_index_to_insert_tone,add_element,rhyme,phone_rhyme);
    	if(rhyme[0]==L'u'&&wcslen(rhyme)>1&&rhyme[1]!=L'n'&&rhyme[1]!=L't'&&rhyme[1]!=L'p'&&rhyme[1]!=L'm'){
    		add_element_for_q(rhyme[0],rhyme,phone_rhyme);
    		insert_tone_and_join_non_end_ptk(find_index_to_insert_tone_for_q,add_element_for_q,rhyme,phone_rhyme);
    	}
    }
}
void insert_tone_and_join_end_ptk(int(*find_index_to_insert_tone_func)(wchar_t*),void(*add_element_func)(wchar_t,wchar_t*,wchar_t*),wchar_t *rhyme,wchar_t *phone_rhyme){
    wchar_t c,first_of_rhyme_non_tone = rhyme[0],temp_rhyme[RHYME_MAX_LENGH];
    int position_replace;
    wcscpy(temp_rhyme,rhyme);
    position_replace = find_index_to_insert_tone_func(temp_rhyme);
    c=temp_rhyme[position_replace];
    switch (c){
        case L'a':
            temp_rhyme[position_replace]=L'á';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ạ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ă':
            temp_rhyme[position_replace]=L'ắ';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ặ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'â':
        	temp_rhyme[position_replace]=L'ấ';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ậ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'e':
        	temp_rhyme[position_replace]=L'é';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẹ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ê':
        	temp_rhyme[position_replace]=L'ế';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ệ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'o':
        	temp_rhyme[position_replace]=L'ó';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ọ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ô':
        	temp_rhyme[position_replace]=L'ố';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ộ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ơ':
        	temp_rhyme[position_replace]=L'ớ';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ợ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'i':
        	temp_rhyme[position_replace]=L'í';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ị';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'u':
        	temp_rhyme[position_replace]=L'ú';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ụ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ư':
        	temp_rhyme[position_replace]=L'ứ';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ự';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'y':
        	temp_rhyme[position_replace]=L'ý';
            wcscat(phone_rhyme,L"-D1");
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỵ';
            phone_rhyme[wcslen(phone_rhyme)-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
    }
}
void insert_tone_and_join_non_end_ptk(int(*find_index_to_insert_tone_func)(wchar_t*),void(*add_element_func)(wchar_t,wchar_t*,wchar_t*),wchar_t *rhyme,wchar_t *phone_rhyme){
    wchar_t c,first_of_rhyme_non_tone = rhyme[0],temp_rhyme[RHYME_MAX_LENGH];
    int position_replace;
    int len = wcslen(phone_rhyme);
    wcscpy(temp_rhyme,rhyme);
    position_replace = find_index_to_insert_tone_func(temp_rhyme);
    c=temp_rhyme[position_replace];
    switch (c){
        case L'a':
        	temp_rhyme[position_replace]=L'á';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ả';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ã';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'à';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ạ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ă':
        	temp_rhyme[position_replace]=L'ắ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẳ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẵ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ằ';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ặ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'â':
            temp_rhyme[position_replace]=L'ấ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            phone_rhyme[len-2]=L'C';
            temp_rhyme[position_replace]=L'ẩ';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẫ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ầ';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ậ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'e':
            temp_rhyme[position_replace]=L'é';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẻ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẽ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'è';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ẹ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ê':
            temp_rhyme[position_replace]=L'ế';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ể';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ễ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ề';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ệ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'o':
            temp_rhyme[position_replace]=L'ó';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỏ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'õ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ò';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ọ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ô':
            temp_rhyme[position_replace]=L'ố';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ổ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỗ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ồ';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ộ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ơ':
            temp_rhyme[position_replace]=L'ớ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ở';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỡ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ờ';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ợ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'i':
            temp_rhyme[position_replace]=L'í';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỉ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ĩ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ì';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ị';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'u':
            temp_rhyme[position_replace]=L'ú';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ủ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ũ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ù';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ụ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'ư':
            temp_rhyme[position_replace]=L'ứ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ử';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ữ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ừ';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ự';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            break;
        case L'y':
            temp_rhyme[position_replace]=L'ý';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỷ';
            phone_rhyme[len-2]=L'C';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỹ';
            phone_rhyme[len-1]=L'2';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỳ';
            phone_rhyme[len-2]=L'A';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
            temp_rhyme[position_replace]=L'ỵ';
            phone_rhyme[len-2]=L'B';
            add_element_func(first_of_rhyme_non_tone,temp_rhyme,phone_rhyme);
    }
}

int is_end_ptk(wchar_t *rhyme){
    int len=wcslen(rhyme);
    switch(rhyme[len-1]){
        case L'p':
            return 1;
        case L't':
            return 1;
        case L'c':
            return 1;
        case L'h':
            if(rhyme[len-2]==L'c')
                return 1;
        default:
            return 0;
    }
}
int find_index_to_insert_tone_for_q(wchar_t *rhyme){
	int len=wcslen(rhyme);
	switch(len){
		case 4:
			if(rhyme[2]==L'ê')
				return 2;
		case 2:
		case 3:
			return 1;
	}
}
int find_index_to_insert_tone(wchar_t *rhyme){
    int len=wcslen(rhyme);
    switch(len){
        case 2:
            if(rhyme[len-1]==L'ơ'||rhyme[len-1]==L'ê')
                return 1;
        case 1:
            return 0;
        case 3:
            if(rhyme[len-1]==L'h'||rhyme[len-1]==L'g')
                return 0;
            return 1;
        case 4:
            if(rhyme[len-1]!=L'h'&&rhyme[len-1]!=L'g')
                return 2;
            return 1;
    }
}

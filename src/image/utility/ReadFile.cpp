#include "image/utility/ReadFile.h"
//���ļ���ÿ�ζ�ȡһ���ߵ����
//(�ļ�·����ÿ�����ж��ٸ�����㡢���ζ�ȡ�ӵڼ����߿�ʼ����ȡ��ݱ��浽����)
signed short int* ReadFile(char* fileName,int deepPoint,int ensemble,long IQOffset){
	FILE* fp = NULL;
	bool bRet = true;
	int head = 1;
	int deepth=5;//����ݵ�����
	signed short int I;
	signed short int Q;
	if (fileName == NULL){
		printf("fileName == NULL\n");
	}
	fp = fopen(fileName,"rb");
	if (fp == NULL){
		bRet == false;
		printf("Read File Error\n");
		exit(1);
	}
	signed short int* IQArray = (signed short int*)malloc((deepPoint*ensemble)*sizeof(signed short int)*2);
	memset(IQArray,0,(deepPoint*ensemble)*sizeof(signed short int)*2);
	fseek(fp,IQOffset*ensemble*2*sizeof(signed short int),SEEK_SET);
	for(int i=0;i<ensemble;i++){
		
			fread(&I,sizeof(signed short int),1,fp);
			fread(&Q,sizeof(signed short int),1,fp);
			for(int j=deepth-2;j<deepth+2;j++){
				IQArray[i*deepPoint*2+j*2]=I;
				IQArray[i*deepPoint*2+j*2+1]=Q;
			}
		
	}
	fclose(fp);
	fp=NULL;
	return IQArray;
}
void PrintArray(int deepPoint,int  ensemble,signed short int* IQArray){
	for (int i=0;i<deepPoint;i++){
			
			for (int j = 0;j<ensemble;j++){
				std::cout<<" ("<<IQArray[j*deepPoint*2+i*2]<<","<<IQArray[j*deepPoint*2+i*2+1]<<") ";
			}
		std::cout<<std::endl;
		}
}

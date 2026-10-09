#include <TFile.h>
#include <TTree.h>

void extract() {
    
// 打开原始ROOT文件
    TFile* inputFile = TFile::Open("ana_data.root", "READ");
    
    // 获取rec Tree
    TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get("rec"));
    
    // 创建新的ROOT文件
    TFile* outputFile = TFile::Open("ana_data_phiome.root", "RECREATE");
    
    // 从原始Tree中复制tagmode=2的事件到新的Tree
    TTree* outputTree = inputTree->CopyTree("tagmode==4&&chisq_4c<200");
    
    // 保存新的ROOT文件
    outputFile->Write();
    outputFile->Close();
    
    // 清理内存
    delete inputFile;
    delete outputFile;
}

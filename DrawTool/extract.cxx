#include <TFile.h>
#include <TTree.h>

void extract() {
    
// 打开原始ROOT文件
    TFile* inputFile = TFile::Open("etacToVV_2009.root", "READ");
    
    // 获取rec Tree
    TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get("rec"));
    
    // 创建新的ROOT文件
    TFile* outputFile = TFile::Open("etacToVV_2_2009.root", "RECREATE");
    
    // 从原始Tree中复制tagmode=2的事件到新的Tree,可在这里进行新的筛选
    TTree* outputTree = inputTree->CopyTree("tagmode == 2 && pid_K[0] > 0.001 && pid_K[1] > 0.001 && pid_pi[2] > 0.001 && pid_pi[3] > 0.001");
    
    // 保存新的ROOT文件
    outputFile->Write();
    outputFile->Close();
    
    // 清理内存
    delete inputFile;
    delete outputFile;
}

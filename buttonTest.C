
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TG3DLine.h>
#include <TApplication.h>
//////////// auxilary class ///////////////////////////////////////////////////
class TextMargin : public TGHorizontalFrame {
  protected:
    TGNumberEntry *fEntry;
  public:
    TextMargin(const TGWindow *p, const char *name) : TGHorizontalFrame(p)
    {
      fEntry = new TGNumberEntry(this, 0, 6, -1, TGNumberFormat::kNESInteger);
      AddFrame(fEntry, new TGLayoutHints(kLHintsLeft));
      TGLabel *label = new TGLabel(this, name);
      AddFrame(label, new TGLayoutHints(kLHintsLeft, 10));
    }
      TGTextEntry *GetEntry() const { return fEntry->GetNumberEntry(); }
      ClassDef(TextMargin, 0)
};
////////////////////////////////////////////////////////////////////////////////
class TrackViewer : public TGMainFrame {
  protected:
    TGTextButton *fButton;   // button being tested
    Int_t event = 0;
    TChain *T;
  public:
    TrackViewer();
    void DoHPosition(Int_t);
    void DoVPosition(Int_t);
    void DoLeftMargin(char*);
    void DoRightMargin(char*);
    void DoTopMargin(char*);
    void DoBottomMargin(char*);
    void setEvent(char*);
    void loadChain();
    ClassDef(TrackViewer, 0)
};
//______________________________________________________________________________
TrackViewer::TrackViewer() : TGMainFrame(gClient->GetRoot(), 10, 10, kHorizontalFrame)
{
  // Main test window.
  SetCleanup(kDeepCleanup);

  //Load in the ROOT files
//  loadChain();

  // Controls on right
  TGVerticalFrame *controls = new TGVerticalFrame(this);
  AddFrame(controls, new TGLayoutHints(kLHintsRight | kLHintsExpandY,5, 5, 5, 5));
  
  // Separator
  TGVertical3DLine *separator = new TGVertical3DLine(this);
  AddFrame(separator, new TGLayoutHints(kLHintsRight | kLHintsExpandY));
  
  // Contents
  TGHorizontalFrame *contents = new TGHorizontalFrame(this);
  AddFrame(contents, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5));
  
  // The button for test
  fButton = new TGTextButton(contents,"&This button has a multi-line label\nand shows features\n"
  "available in the button classes");
  fButton->Resize(300, 200);
  fButton->ChangeOptions(fButton->GetOptions() | kFixedSize);
  fButton->SetToolTipText("The assigned tooltip\ncan be multi-line also",200);
  contents->AddFrame(fButton, new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,20, 20, 20, 20));

  // control margins of the text
  TextMargin *evNum = new TextMargin(controls, "Event Number");
  evNum->GetEntry()->Connect("TextChanged(char*)", "TrackViewer",this, "setEvent(char*)");
  controls->AddFrame(evNum, new TGLayoutHints(kLHintsExpandX)); 

  TGGroupFrame *group = new TGGroupFrame(controls, "Enable/Disable");
  group->SetTitlePos(TGGroupFrame::kCenter);
  TGCheckButton *disable = new TGCheckButton(group, "Switch state\nEnable/Disable");
  disable->SetOn();
  disable->Connect("Toggled(Bool_t)", "TGButton", fButton, "SetEnabled(Bool_t)");
  group->AddFrame(disable, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
  controls->AddFrame(group, new TGLayoutHints(kLHintsExpandX));
  
  // control horizontal position of the text
  TGButtonGroup *cuts = new TGButtonGroup(controls, "Event Cuts");
  cuts->SetTitlePos(TGGroupFrame::kCenter);
  TGCheckButton *Ep = new TGCheckButton(cuts, "E/p");
  TGCheckButton *GC = new TGCheckButton(cuts, "Cherenkov");
  TGCheckButton *trig = new TGCheckButton(cuts, "Trigger (2/5)");
  TGCheckButton *n1 = new TGCheckButton(cuts, "n==1");
  controls->AddFrame(cuts, new TGLayoutHints(kLHintsExpandX)); 
 
  TGTextButton *quit = new TGTextButton(controls, "Quit");
  controls->AddFrame(quit, new TGLayoutHints(kLHintsBottom | kLHintsExpandX,0, 0, 0, 5));
  quit->Connect("Pressed()", "TApplication", gApplication, "Terminate()");
  Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
  
  DontCallClose();
  
  MapSubwindows();
  
  Resize();
  
  SetWMSizeHints(GetDefaultWidth(), GetDefaultHeight(), 1000, 1000, 0 ,0);
  SetWindowName("Button Test");
 
  MapRaised();
}
//______________________________________________________________________________
void TrackViewer::DoHPosition(Int_t id)
{
  // Horizontal position handler.
  Int_t tj = fButton->GetTextJustify();
  tj &= ~kTextCenterX;
  tj &= ~kTextLeft;
  tj &= ~kTextRight;
  tj |= id;
  fButton->SetTextJustify(tj);
}
//______________________________________________________________________________
void TrackViewer::DoVPosition(Int_t id)
{
  // Vertical position handler.
  Int_t tj = fButton->GetTextJustify();
  tj &= ~kTextCenterY;
  tj &= ~kTextTop;
  tj &= ~kTextBottom;
  tj |= id;
  fButton->SetTextJustify(tj);
}
//______________________________________________________________________________
void TrackViewer::DoLeftMargin(char *val)
{
  // Set left text margin.
  fButton->SetLeftMargin(atoi(val));
  gClient->NeedRedraw(fButton);
}
//______________________________________________________________________________
void TrackViewer::DoRightMargin(char *val)
{
  // Set right text margin.
  fButton->SetRightMargin(atoi(val));
  gClient->NeedRedraw(fButton);
}
//______________________________________________________________________________
void TrackViewer::DoTopMargin(char *val)
{
  // Set top text margin.
  fButton->SetTopMargin(atoi(val));
  gClient->NeedRedraw(fButton);
}
//______________________________________________________________________________
void TrackViewer::DoBottomMargin(char *val)
{
  // Set bottom text margin.
  fButton->SetBottomMargin(atoi(val));
  gClient->NeedRedraw(fButton);
}
//______________________________________________________________________________
void TrackViewer::setEvent(char *val){
  event = atoi(val);
  fButton->SetText(Form("The current event is %i",event));
  gClient->NeedRedraw(fButton);
}
//______________________________________________________________________________
void TrackViewer::loadChain(){
  cout << "What run would you like to view?\n";
  Int_t run;
  cin >> run;
  cout << "You asked to view run " << run << endl;

  TString path = "/chafs1/work1/tritium/Rootfiles/";
  TString exp = "tritium";

  if(!gSystem->AccessPathName(TString::Format("%s%s_%d.root",path.Data(),exp.Data(),run),kFileExists)){
    T->Add(TString::Format("%s%s_%d.root",path.Data(),exp.Data(),run));
    cout << "Added file: " << exp << "_" << run << ".root" << endl;
  }else{
    cout << "Requested run has not been replayed. Exiting." << endl << endl;
    gApplication->Terminate();
  }
  Int_t i=1;
  while(!gSystem->AccessPathName(TString::Format("%s%s_%d_%d.root",path.Data(),exp.Data(),run,i),kFileExists)){
    T->Add(TString::Format("%s%s_%d_%d.root",path.Data(),exp.Data(),run,i));
    cout << "Added file: " << exp << "_" << run << "_" << i << ".root" << endl;
    i=i+1;
  }                      

}
////////////////////////////////////////////////////////////////////////////////
void buttonTest()
{
  // Main program.
  new TrackViewer();
}

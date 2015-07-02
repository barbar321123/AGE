#include "Common.h"

//  The purpose of these custom text controls is that you get specified error messages
//  when your mouse cursor focus gets off from a data edit box.

#ifndef AGE_TextControls_h
#define AGE_TextControls_h

class AGEListView: public wxListCtrl
{
public:
    AGEListView(wxWindow *parent, const wxSize &size):
    wxListCtrl(parent, wxID_ANY, wxDefaultPosition, size, wxLC_VIRTUAL | wxLC_REPORT | wxLC_NO_HEADER)
    {
        SetItemCount(0);
        InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 200);
    }

    wxArrayString names;
    vector<int> indexes;

private:
    virtual wxString OnGetItemText(long item, long column) const;
};

class AGELinkedBox
{
public:
    virtual void update(int)=0;
    virtual void enable(bool)=0;
};

class AGETextCtrl: public wxTextCtrl
{
public:
    AGETextCtrl(wxWindow *parent, int width):
    wxTextCtrl(parent, wxID_ANY, "", wxDefaultPosition, wxSize(width, 20), wxTE_PROCESS_ENTER){edits = 0;}

    static AGETextCtrl* init(const ContainerType type, forward_list<AGETextCtrl*> *group,
        wxFrame *frame, const short window, wxWindow *parent, short length = 0);
    virtual int SaveEdits(bool forced = false)=0;
    virtual void update()
    {
        if(container.empty())
        {
            Clear();
            Enable(false);
            if(!LinkedBoxes.empty())
            for(auto it = LinkedBoxes.begin(); it != LinkedBoxes.end(); ++it)
            {
                (*it)->update(-1);
                (*it)->enable(false);
            }
            return;
        }
        curFileLoaded = AGETextCtrl::fileLoaded[window];
        Update();
        Enable(true);
    }
    virtual void Update()=0;
    void changeContainerType(const ContainerType type);
    void clear(){container.clear();}
    void prepend(void* data){container.push_front(data);}
    void resize(int size){container.clear();} // Get rid of this.
    void SetMaxSize(unsigned short size){MaxSize = size;}

    static const wxString BATCHWARNING, BWTITLE, IETITLE;
    // Make these window specific
    static vector<bool> hexMode, accurateFloats;
    static vector<int> unSaved, fileLoaded;
    forward_list<AGELinkedBox*> LinkedBoxes; // These are for check and combo boxes.

protected:
    void OnKillFocus(wxFocusEvent &Event){Event.Skip(); SaveEdits();}
    void OnEnter(wxCommandEvent &Event){SaveEdits(true);}

    bool BatchCheck(string &value, short &batchMode)
    {
        if(value.size() < 3) return false;
        switch((char)value[1])
        {
            case '+': batchMode = 1; value = value.substr(2); return true;
            case '-': batchMode = 2; value = value.substr(2); return true;
            case '*': batchMode = 3; value = value.substr(2); return true;
            case '/': batchMode = 4; value = value.substr(2); return true;
            case '%': batchMode = 5; value = value.substr(2); return true;
            default: return false;
        }
    }
    void HandleResults(int casted)
    {
        if(!LinkedBoxes.empty())
        for(auto it = LinkedBoxes.begin(); it != LinkedBoxes.end(); ++it)
        {
            (*it)->update(casted);
        }
        frame->SetStatusText("Edits: "+lexical_cast<string>(AGETextCtrl::unSaved[window])+" + "+lexical_cast<string>(edits), 3);
        AGETextCtrl::unSaved[window] += edits;
        edits = 0;
    }

    wxFrame* frame;
    ContainerType type;
    forward_list<void*> container; // Change to forward_list
    int curFileLoaded, window, edits;
    unsigned short MaxSize;
};

class TextCtrl_DLL: public wxTextCtrl
{
public:
    TextCtrl_DLL(wxWindow *parent, wxSize dimensions):
    wxTextCtrl(parent, wxID_ANY, "", wxDefaultPosition, dimensions, wxTE_MULTILINE | wxTE_PROCESS_ENTER){}

    int index;
};

class TextCtrl_Byte: public AGETextCtrl
{
public:
    TextCtrl_Byte(wxFrame *frame, int window, wxWindow *parent, bool petit = false):
    AGETextCtrl(parent, petit ? 30 : 50)
    {
        this->frame = frame;
        this->window = window;
        SetBackgroundColour(wxColour(255, 235, 215));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_Byte::OnKillFocus));    // Must-have
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_Byte::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

class TextCtrl_UByte: public AGETextCtrl
{
public:
    TextCtrl_UByte(wxFrame *frame, int window, wxWindow *parent):
    AGETextCtrl(parent, 50)
    {
        this->frame = frame;
        this->window = window;
        SetBackgroundColour(wxColour(255, 235, 215));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_UByte::OnKillFocus));
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_UByte::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

class TextCtrl_Float: public AGETextCtrl
{
public:
    TextCtrl_Float(wxFrame *frame, int window, wxWindow *parent, bool petit = false):
    AGETextCtrl(parent, petit ? 70 : 100)
    {
        this->frame = frame;
        this->window = window;
        SetBackgroundColour(wxColour(255, 225, 255));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_Float::OnKillFocus));
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_Float::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

class TextCtrl_Long: public AGETextCtrl
{
public:
    TextCtrl_Long(wxFrame *frame, int window, wxWindow *parent, bool petit = false):
    AGETextCtrl(parent, petit ? 50 : 100)
    {
        this->frame = frame;
        this->window = window;
        SetBackgroundColour(wxColour(215, 255, 255));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_Long::OnKillFocus));
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_Long::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

class TextCtrl_Short: public AGETextCtrl
{
public:
    TextCtrl_Short(wxFrame *frame, int window, wxWindow *parent, bool petit = false):
    AGETextCtrl(parent, petit ? 50 : 100)
    {
        this->frame = frame;
        this->window = window;
        SetBackgroundColour(wxColour(210, 230, 255));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_Short::OnKillFocus));
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_Short::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

class TextCtrl_UShort: public AGETextCtrl
{
public:
    TextCtrl_UShort(wxFrame *frame, int window, wxWindow *parent):
    AGETextCtrl(parent, 100)
    {
        this->frame = frame;
        this->window = window;
        SetBackgroundColour(wxColour(210, 230, 255));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_UShort::OnKillFocus));
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_UShort::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

class TextCtrl_String: public AGETextCtrl
{
public:
    TextCtrl_String(wxFrame *frame, int window, wxWindow *parent, unsigned short CLength = 0):
    AGETextCtrl(parent, 0)
    {
        this->frame = frame;
        this->window = window;
        MaxSize = CLength;
        SetBackgroundColour(wxColour(220, 255, 220));
        Connect(GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(TextCtrl_String::OnKillFocus));
        Connect(GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(TextCtrl_String::OnEnter));
    }
    int SaveEdits(bool forced = false);
    void Update();
};

#endif

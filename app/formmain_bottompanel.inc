{$ifdef nn}begin end;{$endif}

function TfmMain.IsFocusedBottom: boolean;
begin
  Result:=
    fmConsole.ed.Focused or
    fmConsole.memo.Focused or
    ListboxOut.Focused or
    ListboxVal.Focused;
end;

procedure TfmMain.DoBottom_OnTabClick(Sender: TObject);
var
  SCaption: string;
  i: integer;
begin
  fmConsole.Hide;
  ListboxOut.Hide;
  ListboxVal.Hide;

  //hide panels from app_log(log_panel_add)
  for i:= 0 to FPanelCaptions.Count-1 do
    (FPanelCaptions.Objects[i] as TAppPanelPropsClass).Data.Listbox.Hide;

  //hide panels from app_proc(proc_bottompanel_add)
  for i:= Low(FAppBottomPanels) to High(FAppBottomPanels) do
    with FAppBottomPanels[i] do
      if Assigned(ItemTreeview) then
        ItemTreeview.Hide;

  case TabsBottom.TabIndex of
    0:
      begin
        fmConsole.Show;
        fmConsole.Ed.SetFocus;
      end;
    1:
      begin
        ListboxOut.Show;
        ListboxOut.SetFocus;
      end;
    2:
      begin
        ListboxVal.Show;
        ListboxVal.SetFocus;
      end;
    else
      begin
        SCaption:= TabsBottom.GetTabData(TabsBottom.TabIndex).TabCaption;
        for i:= Low(FAppBottomPanels) to High(FAppBottomPanels) do
          with FAppBottomPanels[i] do
          begin
            if ItemCaption='' then break;
            if Assigned(ItemTreeview) then
            begin
              ItemTreeview.Visible:= ItemCaption=SCaption;
              if PanelBottom.Visible and ItemTreeview.Visible then
                if ItemTreeview.CanFocus then
                  ItemTreeview.SetFocus;
            end;
            if Assigned(ItemListbox) then
            begin
              ItemListbox.Visible:= ItemCaption=SCaption;
              if ItemListbox.Visible then ItemListbox.SetFocus;
            end;
          end;
      end;
  end;
end;

function TfmMain.DoBottom_CaptionToPanelsIndex(const Str: string): integer;
var
  i: integer;
begin
  Result:= -1;
  for i:= Low(FAppBottomPanels) to High(FAppBottomPanels) do
    with FAppBottomPanels[i] do
    begin
      if ItemCaption='' then break;
      if ItemCaption=Str then exit(i);
    end;
end;

function TfmMain.DoBottom_CaptionToControlHandle(const ACaption: string): PtrInt;
var
  Num: integer;
begin
  Num:= DoBottom_CaptionToPanelsIndex(ACaption);
  if Num<0 then
    Result:= 0
  else
    Result:= PtrInt(FAppBottomPanels[Num].ItemTreeview)
end;

function TfmMain.DoBottom_CaptionToTabIndex(const Str: string): integer;
var
  Data: TATTabData;
  i: integer;
begin
  Result:= -1;
  for i:= 0 to TabsBottom.TabCount-1 do
  begin
    Data:= TabsBottom.GetTabData(i);
    if Assigned(Data) then
      if Data.TabCaption=Str then exit(i)
  end;
end;

function TfmMain.DoBottom_AddTab(const ACaption, AControlType: string; ATabIndex: integer): boolean;
const
  cTypeTree = 'tree';
var
  Num: integer;
begin
  if DoBottom_CaptionToPanelsIndex(ACaption)>=0 then exit(false);
  if (AControlType<>cTypeTree) then exit(false);

  Num:= 0;
  while FAppBottomPanels[Num].ItemCaption<>'' do Inc(Num);
  if Num>High(FAppBottomPanels) then exit(false);

  DoSidebar_InitPanelTreeview(FAppBottomPanels[Num], ACaption, PanelBottom);
  TabsBottom.AddTab(ATabIndex, ACaption);
  Result:= true;
end;

function TfmMain.DoBottom_RemoveTab(const ACaption: string): boolean;
var
  Num, i: integer;
begin
  Num:= DoBottom_CaptionToTabIndex(ACaption);
  Result:= Num>=0;
  if Result then
  begin
    fmMain.TabsBottom.DeleteTab(Num, false, false);

    //hard to remov array item, so hide it by "?"
    for i:= Low(FAppBottomPanels) to High(FAppBottomPanels) do
      if FAppBottomPanels[i].ItemCaption=ACaption then
      begin
        FAppBottomPanels[i].ItemCaption:= '?';
        Break;
      end;

    if (Num>0) and (fmMain.TabsBottom.TabIndex=Num) then
      fmMain.TabsBottom.TabIndex:= Num-1;
  end;
end;

function TfmMain.DoBottom_ActivateTab(const ACaption: string): boolean;
var
  Num: integer;
begin
  Num:= DoBottom_CaptionToTabIndex(ACaption);
  Result:= Num>=0;
  if Result then
    fmMain.TabsBottom.TabIndex:= Num;
end;

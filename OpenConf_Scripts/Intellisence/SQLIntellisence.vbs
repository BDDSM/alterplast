'SQLIntellisence
'
'������: $Revision: 1.10 $
'
'SQLIntellisence ��� OpenConf
'	(� trdm)	������ �. �.

' ������ ������� ������ ������� SQLIntellisence
' ���������� ���, ��� �������� ���������� ����� ���� ������� ����� �����,
' ����� $ � �� ������ ������.
Dim IntellisenceExtensions
IntellisenceExtensions = ".txt.ert"

Dim DebugFlag '����������� ���������� ����������
'DebugFlag = True '�������� ����� ���������� ���������
DebugFlag = false '�������� ����� ���������� ���������

const cnstRExWORD = "a-zA-Z�-��-�0-9_"

Dim QueryText ' ����� ���������� �������

Dim RE
Set RE = New RegExp
RE.IgnoreCase = true 
RE.Global = True
RE.Multiline = true

Class CTextPos
	Public SelStartLine
	Public SelStartCol
	Public SelEndLine
	Public SelEndCol
	
	' �����������
	Private Sub Class_Initialize	' Setup Initialize event.
		SelStartLine = 0
		SelStartCol = 0
		SelEndLine = 0
		SelEndCol = 0
	End Sub
	
	' ���������� ������� ���������
	Sub GetDocPos( Doc )
		SelStartLine = doc.SelStartLine
		SelStartCol = doc.SelStartCol
		SelEndLine = doc.SelEndLine
		SelEndCol = doc.SelEndCol
	End Sub
	
	' ������������ ����� ������� �������
	' spPlusStr - ������ ������� �����������, 
	' �������������� ������� ���� ��������������� ������ �� 
	' ������ ������
	Sub CalcNewPos( spPlusStr )
		' �������� ������� ����� ����������.
		tArr = Split(spPlusStr,vbCrLf)
		StrPluss = UBound(tArr) + SelEndLine - SelStartLine
		ColPluss = 0		
		if StrPluss = 1 Then ' 1 ������
			ColPluss = Len( spPlusStr ) + SelEndCol - SelStartCol
		Else '  StrPluss > 1
			ColPluss = Len( tArr(UBound(tArr)) )
		End IF
		if SelEndLine = SelStartLine AND SelEndCol = SelStartCol Then 
			SelEndLine = SelStartLine + StrPluss
			SelEndCol = SelEndCol + ColPluss
			SelStartLine = SelEndLine
			SelStartCol = SelEndCol
		Else '  StrPluss > 1
			SelEndLine = SelStartLine + StrPluss
			SelEndCol = SelEndCol + ColPluss
		End IF
	End Sub
	
	Sub MoveCaret( Doc )
		Doc.MoveCaret SelStartLine, SelStartCol, SelEndLine, SelEndCol
	End Sub
	
End Class

Dim Doc ' ������� �������� 

' �������� ������ ������
Private Function GetSQLQueryAliases(  )

	GetSQLQueryAliases = ""
	RE.Pattern = "(\s+FROM|,)+\s+(["+cnstRExWORD+"\.\$])+\s+as\s(["+cnstRExWORD+"]+)"
	Set Match = RE.Execute(QueryText)
	for each m in Match
		tText = GetLastMatch( m.Value, "(\s+as\s+)(["+cnstRExWORD+"]+)" )
		tArr = Split(tText," ")
		if Len(GetSQLQueryAliases) = 0 Then
			GetSQLQueryAliases = tArr(UBound(tArr))
		Else
			GetSQLQueryAliases = GetSQLQueryAliases +vbcrlf+ tArr(UBound(tArr))
		End If
	Next

	RE.Pattern =  "\s+JOIN+\s+(["+cnstRExWORD+"\$\.])+\s+as\s+(["+cnstRExWORD+"])+"
	Set Match = RE.Execute(QueryText)
	for each m in Match
		tText = GetLastMatch(m.Value,"(\s+as\s+)(["+cnstRExWORD+"]+)")
		tArr = Split(tText," ")
		if Len(GetSQLQueryAliases) = 0 Then
			GetSQLQueryAliases = tArr(UBound(tArr))
		Else
			GetSQLQueryAliases = GetSQLQueryAliases +vbcrlf+ tArr(UBound(tArr))
		End If
	Next

'stop
	RE.Pattern =  "^\s+\)+\s+as\s+(["+cnstRExWORD+"])+"
	Set Match = RE.Execute(QueryText)
	for each m in Match
		tText = GetLastMatch(m.Value,"(\s+as\s+)(["+cnstRExWORD+"]+)")
		tArr = Split(tText," ")
		if Len(GetSQLQueryAliases) = 0 Then
			GetSQLQueryAliases = tArr(UBound(tArr))
		Else
			GetSQLQueryAliases = GetSQLQueryAliases +vbcrlf+ tArr(UBound(tArr))
		End If
	Next

	
End Function


' �������� ������ �������� � �������, ����� �������
' WhatYouHave:
' - 0 ������s
' - 1 �������
' - 2 ��������������
' - 3 ������� ������������ �� "_"
Private Function GetSQLTemplate( WhatYouHave )
	' ��� ����������� � �����:
	' "@" ������� �� 	vbTab + vbCrLf
	' ������ ������ �� "|-" ��� �������, �.�. ��� ��������� �����������, ���� ��������� ���� ������� � ���������...
	GetSQLTemplate = ""
	'TabInd = vbCrLf   + String( doc.SelStartCol, vbTab)
	TabInd = ""
'Stop
	cLine = Doc.Range(doc.SelStartLine)
	RE.Pattern = "^\s*[^\s]{1,}"
	Set Match = RE.Execute(cLine)
	if Match.Count>0 Then
		cLine = Match.Item(0).Value
		if Len(cLine)>1 Then cLine = Mid(cLine,1,Len(cLine)-1) 
	End If
	TabInd = Replace (cLine,"    ", vbTab)
	
	'if doc.SelStartCol>1 Then	TabInd = String( doc.SelStartCol-1, vbTab)
	
	KillNotes = True

	Dim TemplArr(100)
	for i = 0 to UBound(TemplArr)
		TemplArr(i) = ""
	Next 
	if 0 = WhatYouHave Then
		
		TemplArr(0) = "SELECT * "
		TemplArr(1) = "FROM"
		TemplArr(2) = "WHERE"
		TemplArr(3) = "GROUP BY"
		TemplArr(4) = "HAVING "
		TemplArr(5) = "ORDER BY"
		TemplArr(6) = "SELECT * @FROM @WHERE @GROUP BY @HAVING @ORDER BY"
		TemplArr(7) = "BETWEEN :������� AND :�������~"
		TemplArr(8) = "LEFT JOIN $(?) as ON ()"
		TemplArr(9) = "IN (SELECT  FROM #)"
		TemplArr(10) = "UNION ALL"
		TemplArr(11) = "$�����������������.{<��������������> | ���������}.{<������������ | ������������>}(@<���������>, <����>[, <�����>[, <�����������>]])"
		TemplArr(12) = "CASE @WHEN THEN @WHEN THEN @ELSE  END as ..."
		TemplArr(13) = "ORDER BY"
		TemplArr(14) = "(NOLOCK)"
		TemplArr(15) = "INNER JOIN $<���> as <���> on <������� �����������>"
		TemplArr(16) = ""		

	ElseIf 1 = WhatYouHave Then
		KillNotes = False
		TemplArr(0) = "AGV()|- ������� ��������"
		TemplArr(1) = "SUM()|- ������������"
		TemplArr(2) = "MIN()|- ����������� ��������"
		TemplArr(3) = "MAX()|- ������������ ��������"
		TemplArr(4) = "COUNT()|-������� ����� ��������� �����"
		TemplArr(5) = "ASCII()|-�������� ���� �������� ������ �������"
		TemplArr(6) = "CHAR()|-����������� ��� ASCII � ������"
		TemplArr(7) = "SOUNDEX()|-��� ��������� �� 4-� ����"
		TemplArr(8) = "DIFFERENCE()|-������� ���������� ���������"
		TemplArr(9) = "LOWER()|- � ������ �������"
		TemplArr(10) = "UPPER()|-� ������� �������"
		TemplArr(11) = "LTRIM()|-������� ������� ����� �������"
		TemplArr(12) = "RTRIM()|-������� ������� � ����� ������"
		TemplArr(13) = "CHARINDEX()|-����� ��������� � ������"
		TemplArr(14) = "PATINDEX()|-����� ��������� � ������"
		TemplArr(15) = "REPLICATE()"
		TemplArr(16) = "REVERSE()|-"
		TemplArr(17) = "RIGHT()|-"
		TemplArr(18) = "SPACE()|-"
		TemplArr(19) = "STUFF(,,,)|-�������� ������ ������ � ������"
		TemplArr(20) = "SUBSTRING()|-"
		TemplArr(21) = "STR()|-"
		TemplArr(22) = "ACOS()|-"
		TemplArr(23) = "ASIN()|-"
		TemplArr(24) = "ATAN()|-"
		TemplArr(25) = "ATAN2()|-"
		TemplArr(26) = "COS()|-"
		TemplArr(27) = "COT()|-"
		TemplArr(28) = "SIN()|-"
		TemplArr(29) = "TAN()|-"
		TemplArr(30) = "DEGREES()|-"
		TemplArr(31) = "RADIAN()|-"
		TemplArr(32) = "CEILING()|-"
		TemplArr(33) = "FLOOR()|-"
		TemplArr(34) = "EXP()|-"
		TemplArr(35) = "LOG()|-"
		TemplArr(36) = "PI()|-"
		TemplArr(37) = "POWER()|-"
		TemplArr(38) = "ABS()|-"
		TemplArr(39) = "RAND()|-"
		TemplArr(40) = "ROUND()|-"
		TemplArr(41) = "SIGN()|-"
		TemplArr(42) = "SQRT()|-"
		TemplArr(43) = "TEXTPTR()|-"
		TemplArr(44) = "TEXTVALID()|-"
		TemplArr(45) = "CONVERT()|-"
		TemplArr(46) = "STR()|-"
		TemplArr(47) = "DATENAME()|-"
		TemplArr(48) = "DATEPART()|-"
		TemplArr(49) = "DATEADD()|-"
		TemplArr(50) = "DATEDIFF()|-"
		TemplArr(51) = "HOST_NAME()|-"
		TemplArr(52) = "HOST_ID()|-"
		TemplArr(53) = "SUPER_ID()|-"
		TemplArr(54) = "SUPER_NAME()|-"
		TemplArr(55) = "USER_ID()|-"
		TemplArr(56) = "USER_NAME()|-"
		TemplArr(57) = "DB_NAME()|-"
		TemplArr(58) = "DB_ID()|-"
		TemplArr(59) = "GETANSINULL()|-"
		TemplArr(60) = "OBJECT_ID()|-"
		TemplArr(61) = "OBJECT_NAME()|-"
		TemplArr(62) = "INDEX_COL()|-"
		TemplArr(63) = "COL_LENGHT()|-"
		TemplArr(64) = "COL_NAME()|-"
		TemplArr(65) = "DATALENGHT()|-"
		TemplArr(66) = "IDENT_SEED()|-"
		TemplArr(67) = "STATS_DATE()|-"
		TemplArr(68) = "COALESCE()|-������ �� ������� ��������"
		TemplArr(69) = "ISNULL()|-"
		TemplArr(70) = "NULLIF()|-"
		TemplArr(71) = "USER()|-"
		TemplArr(72) = "CURRENT_USER()|-"
		TemplArr(73) = "SESSION_USER()|-"
		TemplArr(74) = "SYSTEM_USER()|-"
		TemplArr(75) = "CURENT_TIMESTAMP()|-"
		TemplArr(76) = "APP_NAME()|-"
		TemplArr(77) = ""
	ElseIf 2 = WhatYouHave Then
		TemplArr(0) = "$��������|- ������������� ������� ��������"
		TemplArr(1) = "$��������13|- ������������� ������� ��������"
		TemplArr(2) = "$����������|- ������������� ���� �������"
		TemplArr(3) = "$������������������|- ������������� ������� ��������"
		TemplArr(4) = "$���������|- ������������� ������� ��������"
		TemplArr(5) = "$��������������36|- ������������� ���� ����������� "
		TemplArr(6) = "$������������|-������������� ���� ���������"
		TemplArr(7) = "$������������36|-������������� ���� ���������"
		TemplArr(8) = "$���������������36|-������������� ���� ������������"
		TemplArr(9) = "$����������������|-������������� �������������� ���������"
		TemplArr(10) = "$����������������|-������������� ������� ����������"
		TemplArr(11) = "$�����������|-������������� ����� ������"
		TemplArr(12) = "$���������|-������������� ���������"
		TemplArr(13) = "$���������|-������������� ����������"
		TemplArr(14) = "$���������������������|-����������� ������� �������� � ��������"
		TemplArr(15) = "$��������������|-����������� ������� ��������"
		TemplArr(16) = "$�����������������"
		TemplArr(17) = "$Constant"
		TemplArr(18) = "$ReferenceKind36"
		TemplArr(19) = "$DocumentKind"
		TemplArr(20) = "$DocumentKind36"
		TemplArr(21) = "$EnumKind36"
		TemplArr(22) = "$PropertyHistory"
		TemplArr(23) = "$DocsJournal"
		TemplArr(24) = "$SelectionColumn"
		TemplArr(25) = "$Calendar"
		TemplArr(26) = "$Holidays"
		TemplArr(27) = "$CalcJournalKind"
		TemplArr(28) = "$Algorithm"
		TemplArr(29) = "$EmptyID"
		TemplArr(30) = "$EmptyID13"
		TemplArr(31) = "$����������"		
		TemplArr(32) = "$��������"		
		TemplArr(33) = "$��������������"		
		TemplArr(34) = "$������������"		
		TemplArr(35) = "$��������������"
		TemplArr(36) = "$�������"
		TemplArr(37) = "$�������������"
		TemplArr(38) = ""
	ElseIf 3 = WhatYouHave Then
		TemplArr(0) = "_1SJOURN"
		TemplArr(1) = "_1SCONNECT"
		TemplArr(2) = "_1SCONST"
		TemplArr(3) = "_1SCRDOC"
		TemplArr(4) = "_1SDNLOCK"
		TemplArr(5) = "_1SSTREAM"
		TemplArr(6) = "_1SSYSTEM"
		TemplArr(7) = "_1SUIDCTL"
		TemplArr(8) = "_1SUSERS"
		TemplArr(9) = ""
	End If

	tText = ""
	for i = 0 To 100
		tTxtOfElem = TemplArr(i)
		pos = InStr(tTxtOfElem,"|-")
		if pos>0 And KillNotes  Then 
			tTxtOfElem = Mid(tTxtOfElem,1,pos-1)
		End If 

		if i = 0 Then
			tText = tTxtOfElem
		Else
			tText = tText + vbCrLf + tTxtOfElem
		End If
		If tTxtOfElem = "" Then Exit For
	Next
	if (WhatYouHave = 2) OR (WhatYouHave = 0) Then '
		tText = tText + GetSQLQueryAliases()
	End If
	'tChoise = CommonScripts.SelectValue(tText)
	tChoise = locSelectValue(tText,"")
	if Len(tChoise)>0 Then 
		pos = InStr(tChoise,"|-")
		if pos>0 Then 
			tChoise = Mid(tChoise,1,pos-1)
		End If 

		tChoise = Replace(tChoise,"@", vbTab + vbCrLf +TabInd+ "|")
	End If
	GetSQLTemplate = tChoise
	
End Function

' ���� ������ �������� ����� � ���� �� ������ � 
' �������� ��� ������ � "| ����� �������"
Private Function GetQueryText( )
	
	IterMax = 750 ' �� ���� ������. ������� �� ����� MaxIter - �����
	Iter = 0
	
	GetQueryText = ""
	StartLine = Doc.SelStartLine 
	NeedGo = True
	While (StartLine<>0 AND NeedGo)
		tLine = Doc.Range(StartLine)
		tLine = ScriptingPlus.TrimEx(tLine)
		if (Left(tLine,1) = "|") OR (Left(tLine,2) = "//") Then 
			if Left(tLine,1) = "|" Then 		
			GetQueryText = tLine + vbCrLf +GetQueryText
			End If		
		Else
			NeedGo = False
		End If		
		StartLine = StartLine - 1
		Iter = Iter + 1
		if (Iter>IterMax) Then NeedGo = False
	
	Wend
	LastStr = Doc.LineCount 
	NeedGo = True
	StartLine = Doc.SelStartLine + 1
	While (StartLine<>LastStr AND NeedGo)
		tLine = Doc.Range(StartLine)
		tLine = ScriptingPlus.TrimEx(tLine)
		if (Left(tLine,1) = "|") OR (Left(tLine,2) = "//") Then 
			if Left(tLine,1) = "|" Then 		
				GetQueryText = GetQueryText + vbCrLf + tLine  
			End If		
		Else
			 NeedGo = False
		End If		
		StartLine = StartLine + 1
		Iter = Iter + 1
		if (Iter>IterMax) Then NeedGo = False
	Wend
	GetQueryText = Replace(GetQueryText,"|","")
	
End Function

' �������� ��������� ���� �� ����������
Private Function GetLastMatch( psLine, psPatern )
	GetLastMatch = ""
	RE.Pattern = psPatern
	Set m = RE.Execute(psLine)
	for each m1 In m
		GetLastMatch = m1.Value
	Next
End Function

' ������ ������ ��������� �������...
Private Function GetSQLMethodFromObj( psObj )
	GetSQLMethodFromObj = ""
	ParentID = ""
	ChildID = ""
	PropsID = ""
	Arr = Split(psObj,".")
	if UBound(Arr)>1 Then Exit Function ' 3 ����� �� ��������������...
	psObjUC = UCase( Arr(0) )
	Select case psObjUC
		Case "_1SJOURN"
			GetSQLMethodFromObj = "ROW_ID,IDJOURNAL,IDDOC,IDDOCDEF,APPCODE,DATE_TIME_IDDOC,DNPREFIX,DOCNO,CLOSED,ISMARK,ACTCNT,VERSTAMP,$�������������"
			if UBound(Arr) = 1 Then
				' ������� ����� ���������.....
				if UCase(Arr(1)) = UCase("$�������������") Then
					Set CommonRekv = MetaData.TaskDef.Childs("����������������������")
					For CRC = 0 To CommonRekv.Count - 1
						GetSQLMethodFromObj = ScriptingPlus.AppendStr(GetSQLMethodFromObj,CommonRekv(CRC).Name,",")
					Next
				End If
			End If
		Case "_1SCONST"
			GetSQLMethodFromObj = "ROW_ID,OBJID,ID,DATE,VALUE,DOCID,TIME,ACTNO,LINENO_,TVALUE"
		Case "_1SCRDOC"
			GetSQLMethodFromObj = "MDID,PARENTVAL,CHILD_DATE_TIME_IDDOC,CHILDID,FLAGS"
		Case "$������������"
			ParentID = "��������"
		Case "$��������"
			ParentID = "��������"
			if UBound(Arr)=1 Then	
				GetSQLMethodFromObj = "IDDOC"
				ChildID = Arr(1)
				PropsID = "�������������"
			End If
		Case "$��������������"
			ParentID = "��������"
			if UBound(Arr)=1 Then	
				GetSQLMethodFromObj = "IDDOC,LINENO_"
				ChildID = Arr(1)
				PropsID = "����������������������"				
			End If
		Case "$��������������"
			ParentID = "�������"
			if UBound(Arr)=1 Then	
				GetSQLMethodFromObj = "PERIOD"
				ChildID = Arr(1)
				PropsID = "���������,������"				
			End If
		Case "$�������"
			ParentID = "�������"
			if UBound(Arr)=1 Then	
				GetSQLMethodFromObj = "IDDOC,LINENO_,ACTNO,DEBKRED"
				ChildID = Arr(1)
				PropsID = "���������,������,��������"				
			End If
			
		Case "$����������"
			ParentID = "����������"
			if UBound(Arr)=1 Then	
				GetSQLMethodFromObj = "ROW_ID,ID,ISMARK,VERSTAMP"
				ChildID = Arr(1)
				PropsID = "��������"
				' ������� ����, � ����������� �� ��������� �����������....
			End If
		Case "$������������"
			ParentID = "������������"
		Case "$�������������"
			ParentID = "����������������������"			
	End Select
	If Len(ParentID)>0 Then
		Set Childs = MetaData.TaskDef.Childs(CStr(ParentID))
		For i = 0 To Childs.Count - 1			
			Set mdo = Childs(i)
			if Len(ChildID)>0 Then 
				if UCase(ChildID) = UCase(mdo.Name) Then
					' ���� ������� ������ ����������, �� �� ���� � �������������� ����
					' ���������......
					Select case psObjUC
						Case "$����������"
							' ��������� ��� � ���� � �������						
							if mdo.Props(5)>1 Then  ScriptingPlus.AppendStr GetSQLMethodFromObj,"CODE","," ' ��������� + CODE						
							if mdo.Props(6)>1 Then  ScriptingPlus.AppendStr GetSQLMethodFromObj,"DESCR","," ' ��������� + DESCR						
							if Len(mdo.Props(3))>0 Then  ScriptingPlus.AppendStr GetSQLMethodFromObj,"PARENTEXT","," ' �������� + PARENTEXT						
							if mdo.Props(4)>0 Then  ScriptingPlus.AppendStr GetSQLMethodFromObj,"PARENTID,ISFOLDER","," ' ���������� ������� + PARENTID,ISFOLDER
					End Select
					ArrPropsName = Split(PropsID,",")
					For iii=0 To UBound(ArrPropsName)
						Set SubChild = mdo.Childs(CStr(Trim(ArrPropsName(iii))))
						For ii=0 To SubChild.Count-1
							Set mdo1 = SubChild(ii)
							GetSQLMethodFromObj = ScriptingPlus.AppendStr(GetSQLMethodFromObj,mdo1.Name,",")
						Next
					Next
					exit for
				End If			
			else
				if Len(GetSQLMethodFromObj) = 0 Then
					GetSQLMethodFromObj = mdo.Name
				Else
					GetSQLMethodFromObj = GetSQLMethodFromObj + "," + mdo.Name
				End If
			End If
		Next
	End IF
End Function


' ��� ��� ����������� � ����������..
Private Function GetSQLMethod( cLine )

	strMetod = ""
	GetSQLMethod = ""
'stop	
	'cObj = GetLastMatch(cLine,"\$+(["+cnstRExWORD+"])+\.$")
	cObj = GetLastMatch(cLine,"\$+(["+cnstRExWORD+"\.])+\.$")
	if Len(cObj)>0 Then ' ���� �����
		tLen = Len(cObj)
		If tLen>2 Then 
			cObj = Mid(cObj, 1,tLen-1) ' ��� ����� - �� �����������
		Else
			Exit Function
		End If	
		strMetod = GetSQLMethodFromObj(cObj)
	End If
	if Len(strMetod)=0 Then ' ���� �����
		cObj = GetLastMatch(cLine,"(["+cnstRExWORD+"])+\.")
		tLen = Len(cObj)
		If tLen>1 Then cObj = Mid(cObj, 1,tLen-1)
		' ������� �����������: 	"|FROM *** AS *** " ��� ", *** AS ***" ���������� ������� ���������

		tPaternFrom =  "(\s+FROM|,|join)+\s+(["+cnstRExWORD+"\$\.])+\s+as\s+("+cObj+")+(\s|\()"
		HowBorn = GetLastMatch(QueryText,tPaternFrom)
		if Len(HowBorn)>0 Then
			tPaternFrom =  "(["+cnstRExWORD+"\$\.]+)(?=\s+as\s+)"
			HowBorn = GetLastMatch( HowBorn, tPaternFrom)
			strMetod = GetSQLMethodFromObj(HowBorn) ' ������ ���� ���-�� �������....
		End If	
	End If
	
	if Len(strMetod) = 0 Then 
		' ������� �����������: 	"|FROM *** AS *** " ��� ", *** ***" ���������� �� ������� ���������
		tPaternFrom =  "(\s+FROM|,|join)+\s+(["+cnstRExWORD+"\$\.])+\s+("+cObj+")+(\s|\()"
		HowBorn = GetLastMatch(QueryText,tPaternFrom)
		if Len(HowBorn)>0 Then
			tPaternFrom =  "(["+cnstRExWORD+"\$\.]+)(?=\s+)"
			HowBorn = GetLastMatch( HowBorn, tPaternFrom)
			strMetod = GetSQLMethodFromObj(HowBorn) ' ������ ���� ���-�� �������....
		End If	
	End If			

	if Len(strMetod) = 0 Then  ' ������� ������ ������ �������������...
			'JOIN $��������.������������������	as �����
			tPaternFrom =  "\s+JOIN+\s+(["+cnstRExWORD+"\$\.])+\s+as\s"+cObj
			HowBorn = GetLastMatch(QueryText,tPaternFrom)
			if Len(HowBorn)>0 Then 
				tPaternFrom =  "(["+cnstRExWORD+"\$\.]+)(?=\s+as\s+)"
				HowBorn = GetLastMatch( HowBorn, tPaternFrom)
				strMetod = GetSQLMethodFromObj(HowBorn) ' ������ ���� ���-�� �������....
				'strMetod = GetSQLMethodFromObj(HowBorn) ' ������ ���� ���-�� �������....
			End If
		End If
	if Len(strMetod) = 0 Then 
		'JOIN $��������.������������������ �����
		tPaternFrom =  "\s+JOIN+\s+(["+cnstRExWORD+"\$\.])+\s+"+cObj+"(\s|,)"
		HowBorn = GetLastMatch(QueryText,tPaternFrom)
		if Len(HowBorn)>0 Then 
			tPaternFrom =  "(["+cnstRExWORD+"\$\.]+)\s+"
			HowBorn = GetLastMatch( HowBorn, tPaternFrom)
			HowBorn = Trim(HowBorn)
			strMetod = GetSQLMethodFromObj(HowBorn) ' ������ ���� ���-�� �������....
			'strMetod = GetSQLMethodFromObj(HowBorn) ' ������ ���� ���-�� �������....
	End If
	End If			

	if Len(strMetod)>0 Then
		strMetod = Replace(strMetod,",",vbcrlf)
		GetSQLMethod = locSelectValue(strMetod,"")
	End If
	
End Function

Private Function GetDLGFields()
	GetDLGFields = ""
	strDLGFields = ""
	DocWB = ""
	strNeedCtrlType = LCase(",��������,����������,������,�������������,")
	if not CheckWorkBook(DocWB) Then Exit Function
	On Error Resume Next
	
	for i = 0 to DocWB.ctrlCount - 1
		if InStr(1,strNeedCtrlType,","+LCase(DocWB.ctrlType(i))+",") > 0 Then
			strDLGFields = ScriptingPlus.AppendStr(strDLGFields,DocWB.ctrlProp(i,cpStrID),",")
		End If
	Next
	
	on error goto 0

	if Len(strDLGFields)>0 Then
		strDLGFields = Replace(strDLGFields,",",vbcrlf)
		'strDLGFields = CommonScripts.SelectValue(strDLGFields)		
		strDLGFields = locSelectValue(strDLGFields,"")
	End If
	GetDLGFields = strDLGFields
End Function

private Function locSelectValue (psStrValues, psAddParam)
	locSelectValue = ""
	HasError = false
	On Error resume next 
	locSelectValue = Scripts("Intellisence").SelectFromList(psStrValues, "")
	if err.number<>0 Then HasError = true
	on error goto 0
	if HasError Then 
		Set srv=CreateObject("Svcsvc.Service")
		locSelectValue = srv.FilterValue(psStrValues,5+16,Caption) '����������� + ���� ����� ��������
	End If
	
End Function


' �������� ������, ������ 1 ����� 0
Function RunSQLIntellisence()
	RunSQLIntellisence = 0
	Doc = ""
	if Not CheckWindow(doc) then Exit Function

	cLine = doc.Range (doc.SelStartLine,1, doc.SelEndLine, doc.SelEndCol)
	cLine = ScriptingPlus.TrimEx(cLine)

	Set CPos = New CTextPos
	CPos.GetDocPos Doc
'Stop

	QueryText = GetQueryText()

'Stop

	cChoise = "" ' ������� ����� ������������ 
	if cLine = "|" or Right(cLine,1) = "," Then ' ���� ������ � ��� "������" ��������� ������ ��������....
		cChoise = GetSQLTemplate(0)
	ElseIf Right(cLine,1) = "$" Then ' ����������� ������ ���������������
		cChoise = GetSQLTemplate(2)		
		cChoise = Replace(cChoise,"$","")
	ElseIf Right(cLine,1) = "." Then ' ��������� ���������� ��� � ��� �������
		cChoise = GetSQLMethod(cLine)
	ElseIf Right(cLine,1) = "*" Then ' ������� ������ �������
		CPos.SelStartCol = CPos.SelStartCol - 1 ' �������� ���������
		CPos.MoveCaret Doc
		cChoise = GetSQLTemplate(1)
	ElseIf Right(cLine,1) = ":" Then ' ������� ������ ��������� � �����������
		cChoise = GetDLGFields()
	ElseIf Right(cLine,1) = "_" Then ' ������� ������ ������, � ������� �������� ���������� � "_">>"_1SJourn"
		CPos.SelStartCol = CPos.SelStartCol - 1 ' �������� "_"
		CPos.MoveCaret Doc
		cChoise = GetSQLTemplate( 3 )
	Else							 ' ��������� ������������ �����-�������
		' ���� ��������� ������ ��� ������������� ���� "��"->"as" � ������ 
		' 
		' �� ��� �����...
	End If
	if Len(cChoise) > 0 Then
		CPos.CalcNewPos cChoise 
		Doc.Range(doc.SelStartLine,doc.SelStartCol ,doc.SelEndLine ,doc.SelEndCol ) = cChoise
		CPos.SelStartCol = CPos.SelEndCol
		CPos.MoveCaret Doc
		RunSQLIntellisence = 1
	End If
End Function


':::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'���������� True ���� ������� ���� - ���������
'
private Function CheckWindow(doc)
	CheckWindow = False

	set doc = CommonScripts.GetTextDocIfOpened(0)
	if doc is nothing then exit Function
	' ������� �������� �� �� ������ � ������, ����� ������� �����.
	if CommonScripts.FSO.FileExists(Doc.path) Then
		' ��������� ���������� ������ ���� � ��� ���� ������ � ������....
		if Not CommonScripts.CheckDocOnExtension(doc, IntellisenceExtensions) Then Exit Function
	End If
	CheckWindow = True
End Function

'=============================================================================================
' ��������� �������� ���� �� �������������� � WorkBook � ���������� 
' Windows.ActiveWnd.Document.Page(0)
Function CheckWorkBook(doc) 
	CheckWorkBook = False
	If Windows.ActiveWnd Is Nothing Then
    	Exit Function
	End If   
	Set doc = Windows.ActiveWnd.Document
	If doc<>docWorkBook Then Exit Function
	Set doc=doc.Page(0)  
	CheckWorkBook = True
End Function  


Private Sub InitScript( p )
	Set c = Nothing
	On Error Resume Next
	Set c = CreateObject("OpenConf.CommonServices")
	On Error GoTo 0
	If c Is Nothing Then
   		Message "�� ���� ������� ������ OpenConf.CommonServices", mRedErr
   		Message "������ " & SelfScript.Name & " �� ��������", mInformation
   		Scripts.UnLoad SelfScript.Name
		Exit Sub
	End If
	c.SetConfig(Configurator)

	'' ��������: ��������� 2 ������ ����������� � ����� �������, ����� �� ���������� !
	'
	'' ��� �������� ������� �������������� ���
	'c.AddPluginToScript SelfScript, "�������", "Telepat", Telepat

	SelfScript.AddNamedItem "CommonScripts", c, False

	' ��������� �������
	CommonScripts.SetQuietMode not DebugFlag
	
	Set sp = Nothing
	On Error Resume Next
	Set sp = CreateObject("ScriptingPlus.Function")
	On Error GoTo 0
	If sp Is Nothing Then
   		Message "�� ���� ������� ������ ScriptingPlus.Function", mRedErr
   		Message "������ " & SelfScript.Name & " �� ��������", mInformation
   		Scripts.UnLoad SelfScript.Name
		Exit Sub
	End If
	SelfScript.AddNamedItem "ScriptingPlus", sp, False


End Sub

InitScript 0
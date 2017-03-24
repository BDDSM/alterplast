	enum ClassesOfSymbol{
		CLS_WHTSPC,		// ' ' \t
		CLS_REMARK,		// '/'
		CLS_QUOTE,		// '"'
		CLS_QUOTECONT,	// '|'
		CLS_LPAR,			// '('
		CLS_RPAR,			// ')'
		CLS_LBRACKET,		// '['
		CLS_RBRACKET,		// ']'
		CLS_COMMA,		// ','
		CLS_SEMICOLON,	// ';'
		CLS_EQUAL,		// '='
		CLS_CR,			//
		CLS_LF,			//
		CLS_PLUS,			// '+'
		CLS_MINUS,		// '-'
		CLS_MULT,			// '*'
		CLS_PERCENT,		// '%'
		CLS_TILDA,		// '~'
		CLS_COLON,		// ':'
		CLS_LESS,			// '<'
		CLS_GRAT,			// '>'
		CLS_APOSTROF,		// '''
		CLS_PERIOD,		// '.'
		CLS_NUMBER,		//
		CLS_SYMBOL,
		CLS_A,
		CLS_B,
		CLS_C,
		CLS_D,
		CLS_E,
		CLS_F,
		CLS_G,
		CLS_H,
		CLS_I,
		CLS_K,
		CLS_L,
		CLS_N,
		CLS_O,
		CLS_P,
		CLS_R,
		CLS_S,
		CLS_T,
		CLS_U,
		CLS_V,
		CLS_Y,
		CLS_W,
		CLS_X,
		CLS_R_A,	// �
		CLS_R_V,	// �
		CLS_R_G,	// �
		CLS_R_D,	// �
		CLS_R_E,	// �
		CLS_R_ZH,	// �
		CLS_R_Z,	// �
		CLS_R_I,	// �
		CLS_R_IY,	// �
		CLS_R_K,	// �
		CLS_R_L,	// �
		CLS_R_M,	// �
		CLS_R_N,	// �
		CLS_R_O,	// �
		CLS_R_P,	// �
		CLS_R_R,	// �
		CLS_R_S,	// �
		CLS_R_T,	// �
		CLS_R_U,	// �
		CLS_R_C,	// �
		CLS_R_4,	// �
		CLS_R_F,	// �
		CLS_R_MZ,	// �
		CLS_R_Y,	// �
		CLS_R_e,	// �
		CLS_R_YU,	// �
		CLS_R_YA,	// �
		CLS_WHAT,	// ?
		CLS_J,
		CLS_R_B,
		CLS_R_TZ,
		CLS_LAST
	};
	enum States{
		STATE_NONE,
		// �������
		STATE_R_V,
		STATE_R_VO,
		STATE_R_VOZ,
		STATE_R_VOZV,
		STATE_R_VOZVR,
		STATE_R_VOZVRA,
		// �����
		STATE_R_D,
		STATE_R_DA,
		STATE_R_DAL,
		STATE_R_DALE,
		// ���
		STATE_R_DL,
		STATE_R_DLYA,
		// ����
		STATE_R_E,
		STATE_R_ES,
		STATE_R_ESL,
		// ����
		STATE_R_Z,
		STATE_R_ZN,
		STATE_R_ZNA,
		// �
		STATE_R_I,
		// ���
		STATE_R_IL,
		// �����
		STATE_R_IN,
		STATE_R_INA,
		STATE_R_INA4,
		STATE_R_INA4E,
		// ���������
		STATE_R_INA4EE,
		STATE_R_INA4EES,
		STATE_R_INA4EESL,
		// ����������
		STATE_R_IS,
		STATE_R_ISK,
		STATE_R_ISKL,
		STATE_R_ISKLU,
		STATE_R_ISKLU4,
		STATE_R_ISKLU4E,
		STATE_R_ISKLU4EN,
		STATE_R_ISKLU4ENI,
		// ���������
		STATE_R_K,
		STATE_R_KO,
		STATE_R_KON,
		STATE_R_KONE,
		STATE_R_KONEC,
		STATE_R_KONECE,
		STATE_R_KONECES,
		STATE_R_KONECESL,
		// ������������
		STATE_R_KONECP,
		STATE_R_KONECPO,
		STATE_R_KONECPOP,
		STATE_R_KONECPOPY,
		STATE_R_KONECPOPYT,
		STATE_R_KONECPOPYTK,
		// ��������������
		STATE_R_KONECPR,
		STATE_R_KONECPRO,
		STATE_R_KONECPROC,
		STATE_R_KONECPROCE,
		STATE_R_KONECPROCED,
		STATE_R_KONECPROCEDU,
		STATE_R_KONECPROCEDUR,
		// ������������
		STATE_R_KONECF,
		STATE_R_KONECFU,
		STATE_R_KONECFUN,
		STATE_R_KONECFUNK,
		STATE_R_KONECFUNKC,
		STATE_R_KONECFUNKCI,
		// ����������
		STATE_R_KONECC,
		STATE_R_KONECCI,
		STATE_R_KONECCIK,
		STATE_R_KONECCIKL,
		// ��
		STATE_R_N,
		// �����
		STATE_R_P,
		STATE_R_PE,
		STATE_R_PER,
		STATE_R_PERE,
		// �������
		STATE_R_PEREY,
		STATE_R_PEREYT,
		// ��
		STATE_R_PO,
		// ����
		STATE_R_POK,
		// �������
		STATE_R_POP,
		STATE_R_POPY,
		STATE_R_POPYT,
		STATE_R_POPYTK,
		// ��������
		STATE_R_PR,
		STATE_R_PRE,
		STATE_R_PRER,
		STATE_R_PRERV,
		STATE_R_PRERVA,
		STATE_R_PRERVAT,
		// ����������
		STATE_R_PRO,
		STATE_R_PROD,
		STATE_R_PRODO,
		STATE_R_PRODOL,
		STATE_R_PRODOLZH,
		STATE_R_PRODOLZHI,
		STATE_R_PRODOLZHIT,
		// ���������
		STATE_R_PROC,
		STATE_R_PROCE,
		STATE_R_PROCED,
		STATE_R_PROCEDU,
		STATE_R_PROCEDUR,
		// �����
		STATE_R_T,
		STATE_R_TO,
		STATE_R_TOG,
		STATE_R_TOGD,
		// �������
		STATE_R_F,
		STATE_R_FU,
		STATE_R_FUN,
		STATE_R_FUNK,
		STATE_R_FUNKC,
		STATE_R_FUNKCI,
		// ����
		STATE_R_C,
		STATE_R_CI,
		STATE_R_CIK,
		// �������
		STATE_R_e,
		STATE_R_EK,
		STATE_R_EKS,
		STATE_R_EKSP,
		STATE_R_EKSPO,
		STATE_R_EKSPOR,
		// And
		STATE_A,
		STATE_AN,
		// Break
		STATE_B,
		STATE_BR,
		STATE_BRE,
		STATE_BREA,
		// Continue
		STATE_C,
		STATE_CO,
		STATE_CON,
		STATE_CONT,
		STATE_CONTI,
		STATE_CONTIN,
		STATE_CONTINU,
		// Do
		STATE_D,
		// Else
		STATE_E,
		STATE_EL,
		STATE_ELS,
		// Elsif
		STATE_ELSI,
		// EndDo
		STATE_EN,
		STATE_END,
		STATE_ENDD,
		// EndIf
		STATE_ENDI,
		// EndProcedure
		STATE_ENDP,
		STATE_ENDPR,
		STATE_ENDPRO,
		STATE_ENDPROC,
		STATE_ENDPROCE,
		STATE_ENDPROCED,
		STATE_ENDPROCEDU,
		STATE_ENDPROCEDUR,
		// EndFunction
		STATE_ENDF,
		STATE_ENDFU,
		STATE_ENDFUN,
		STATE_ENDFUNC,
		STATE_ENDFUNCT,
		STATE_ENDFUNCTI,
		STATE_ENDFUNCTIO,
		// EndTry
		STATE_ENDT,
		STATE_ENDTR,
		// except
		STATE_EX,
		STATE_EXC,
		STATE_EXCE,
		STATE_EXCEP,
		// export
		STATE_EXP,
		STATE_EXPO,
		STATE_EXPOR,
		// for
		STATE_F,
		STATE_FO,
		STATE_FOR,
		// forward
		STATE_FORW,
		STATE_FORWA,
		STATE_FORWAR,
		// function
		STATE_FU,
		STATE_FUN,
		STATE_FUNC,
		STATE_FUNCT,
		STATE_FUNCTI,
		STATE_FUNCTIO,
		// goto
		STATE_G,
		STATE_GO,
		STATE_GOT,
		// if
		STATE_I,
		// not
		STATE_N,
		STATE_NO,
		// or
		STATE_O,
		// procedure
		STATE_P,
		STATE_PR,
		STATE_PRO,
		STATE_PROC,
		STATE_PROCE,
		STATE_PROCED,
		STATE_PROCEDU,
		STATE_PROCEDUR,
		// return
		STATE_R,
		STATE_RE,
		STATE_RET,
		STATE_RETU,
		STATE_RETUR,
		// then, to
		STATE_T,
		STATE_TH,
		STATE_THE,
		// try
		STATE_TR,
		// val,var
		STATE_V,
		STATE_VA,
		// while
		STATE_W,
		STATE_WH,
		STATE_WHI,
		STATE_WHIL,

		STATE_R_S,
		STATE_R_SO,
		STATE_R_SOZ,
		STATE_R_SOZD,
		STATE_R_SOZDA,
		STATE_R_SOZDAT,
		STATE_R_SOZDATT,
		STATE_R_SOZDATTO,
		STATE_R_SOZDATTOB,
		STATE_R_SOZDATTOBB,
		STATE_R_SOZDATTOBBE,
		STATE_R_SOZDATTOBBEC,

		STATE_CR,
		STATE_CRE,
		STATE_CREA,
		STATE_CREAT,
		STATE_CREATE,
		STATE_CREATEO,
		STATE_CREATEOB,
		STATE_CREATEOBJ,
		STATE_CREATEOBJE,
		STATE_CREATEOBJEC,
		STATE_CREATEOBJECT,

		STATE_R_KONT,
		STATE_R_KONTE,
		STATE_R_KONTEK,
		STATE_R_KONTEKS,

		STATE_CONTE,
		STATE_CONTEX,

		// keywords
		STATE_AND,
		STATE_RETURN,
		STATE_FORWARD,
		STATE_IF,
		STATE_VAL,
		STATE_OR,
		STATE_ELSE,
		STATE_ELSIF,
		STATE_EXCEPT,
		STATE_ENDIF,
		STATE_ENDTRY,
		STATE_ENDPROCEDURE,
		STATE_ENDFUNCTION,
		STATE_ENDDO,
		STATE_NOT,
		STATE_VAR,
		STATE_GOTO,
		STATE_TO,
		STATE_WHILE,
		STATE_TRY,
		STATE_BREAK,
		STATE_CONTINUE,
		STATE_PROCEDURE,
		STATE_THEN,
		STATE_FUNCTION,
		STATE_EXPORT,
		STATE_DO,
		STATE_CONTEXT,

		STATE_NAME,

		STATE_NUM,

		STATE_DIV,
		STATE_REMARK,

		STATE_QUOT1,
		STATE_QUOTE,

		STATE_DAT1,
		STATE_DATE,
		
		STATE_LPAR,
		STATE_RPAR,
		STATE_COMMA,
		STATE_SEMICOLON,
		STATE_EQUAL,
		STATE_LBRACKET,
		STATE_RBRACKET,
		STATE_PLUS,
		STATE_MINUS,
		STATE_MULT,
		STATE_PERCENT,
		STATE_TILDA,
		STATE_LABEL,
		STATE_COLON,
		STATE_LESS,
		STATE_GRAT,
		STATE_PERIOD,
		STATE_WHAT,
		STATE_NUMPERIOD,

		STATE_EOF,
		STATE_LAST
	};
#if 0
�������				STATE_RETURN
�����				STATE_FORWARD
���					STATE_FOR
����				STATE_IF
����				STATE_VAL
�					STATE_AND
���					STATE_OR
�����				STATE_ELSE
���������			STATE_ELSIF
����������			STATE_EXCEPT
���������			STATE_ENDELSE
������������		STATE_ENDTRY
��������������		STATE_ENDPROC
������������		STATE_ENDFUNC
����������			STATE_ENDDO
He					STATE_NOT
�����				STATE_VAR
�������				STATE_GOTO
��					STATE_TO
����				STATE_WHILE
�������				STATE_TRY
��������			STATE_BREAK
����������			STATE_CONTINUE
���������			STATE_PROCEDURE
�����				STATE_THEN
�������				STATE_FUNCTION
����				STATE_DO
�������				STATE_EXPORT

And
Break
Continue
Do
Else			
Elsif			
EndDo
Endlf
EndProcedure
EndFunction		
endtry
except
Export			
For
Forward			
Function		
Goto
If				
Not
Or
Procedure		
Return			
Then			
To
try
Val
Var
While
#endif

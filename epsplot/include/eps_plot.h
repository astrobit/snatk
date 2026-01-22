#pragma once
#include <cstring>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
#include <map>
#include <string>

namespace	epsplot
{
	enum PS_FONT {TIMES,HELVETICA,COURIER,SYMBOL};
	enum PS_HORIZONTAL_JUSTIFICATION {LEFT,CENTER,RIGHT};
	enum PS_VERTICAL_JUSTIFICATION {TOP,MIDDLE,BOTTOM};
	enum PS_STANDARD_COLORS { STD_BLACK,STD_WHITE,STD_RED,STD_GREEN,STD_BLUE,STD_CYAN,STD_MAGENTA,STD_YELLOW};
	enum COLOR {BLACK, RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, GREY_25, GREY_50, GREY_75, GRAY_25, GRAY_50, GRAY_75, WHITE, CLR_CUSTOM_1, CLR_CUSTOM_2, CLR_CUSTOM_3, CLR_CUSTOM_4, CLR_CUSTOM_5, CLR_CUSTOM_6, CLR_CUSTOM_7, CLR_CUSTOM_8, CLR_CUSTOM_9, CLR_CUSTOM_10, CLR_CUSTOM_11, CLR_CUSTOM_12, CLR_CUSTOM_13, CLR_CUSTOM_14, CLR_CUSTOM_15, CLR_CUSTOM_16};
	enum STIPPLE {SOLID, SHORT_DASH, LONG_DASH, LONG_SHORT_DASH, DOTTED, SHORT_DASH_DOTTED, LONG_DASH_DOTTED, LONG_SHORT_DASH_DOTTED, LONG_LONG_DASH, STPL_CUSTOM_1, STPL_CUSTOM_2, STPL_CUSTOM_3, STPL_CUSTOM_4, STPL_CUSTOM_5, STPL_CUSTOM_6, STPL_CUSTOM_7, STPL_CUSTOM_8, STPL_CUSTOM_9, STPL_CUSTOM_10, STPL_CUSTOM_11, STPL_CUSTOM_12, STPL_CUSTOM_13, STPL_CUSTOM_14, STPL_CUSTOM_15, STPL_CUSTOM_16};
	enum SYMBOL_TYPE {SQUARE, CIRCLE, TRIANGLE_UP, TRIANGLE_DOWN, TRIANGLE_LEFT, TRIANGLE_RIGHT, DIAMOND, TIMES_SYMB, PLUS_SYMB, DASH_SYMB, ASTERISK_SYMB, STAR4, STAR5, STAR6, SYMB_CUSTOM_1, SYMB_CUSTOM_2, SYMB_CUSTOM_3, SYMB_CUSTOM_4, SYMB_CUSTOM_5, SYMB_CUSTOM_6, SYMB_CUSTOM_7, SYMB_CUSTOM_8, SYMB_CUSTOM_9, SYMB_CUSTOM_10, SYMB_CUSTOM_11, SYMB_CUSTOM_12, SYMB_CUSTOM_13, SYMB_CUSTOM_14, SYMB_CUSTOM_15, SYMB_CUSTOM_16};
	enum ERRORBAR_DIRECTION {ERRORBAR_X_LEFT, ERRORBAR_X_RIGHT, ERRORBAR_Y_UPPER, ERRORBAR_Y_LOWER};
	enum ERRORBAR_TIP_TYPE {ERRORBAR_TIP_LINE, ERRORBAR_TIP_ARROW, ERRORBAR_TIP_LINE_AND_ARROW};
	enum	AXIS {X_AXIS,Y_AXIS,Z_AXIS};
	enum z_axis_scheme {rainbow,inverse_rainbow,dark_to_light,light_to_dark,two_color_transition};
	enum z_axis_iterpolation_scheme {nearest,inverse_distance_weight_1,inverse_distance_weight_2,inverse_distance_weight_3,inverse_distance_weight_4,inverse_distance_weight_5,inverse_distance_weight_6,inverse_distance_weight_7,inverse_distance_weight_8,inverse_distance_weight_9,inverse_distance_weight_10,inverse_distance_weight_11,inverse_distance_weight_12,inverse_distance_weight_13,inverse_distance_weight_14,inverse_distance_weight_15,inverse_distance_weight_16};

	class symbol_type
	{
	private:
		size_t m_tCurr_Idx;
		std::vector<SYMBOL_TYPE> m_vType_Cycle;
	public:
		symbol_type(void);
		void reset(void);
		std::vector<SYMBOL_TYPE> getTypeCycle(void) const;
		void setTypeCycle(const std::vector<SYMBOL_TYPE> &i_vCycle);
		SYMBOL_TYPE get(void) const;
		operator SYMBOL_TYPE() const;
		void set(SYMBOL_TYPE i_eType);
		symbol_type operator =(SYMBOL_TYPE i_eType);
		bool operator ==(const symbol_type & i_cRHO) const;
		bool operator ==(const SYMBOL_TYPE & i_eRHO) const;
		symbol_type & operator++(int);
		symbol_type & operator--(int);
		symbol_type & operator++(void);
		symbol_type & operator--(void);
		symbol_type & operator+=(size_t i_tDelta);
		symbol_type & operator-=(size_t i_tDelta);
		symbol_type operator+(size_t i_tDelta) const;
		symbol_type operator-(size_t i_tDelta) const;
	};
		

	class color
	{
	private:
		size_t m_tCurr_Idx;
		std::vector<COLOR> m_vType_Cycle;
	public:
		color(void);
		void reset(void);
		std::vector<COLOR> getTypeCycle(void) const;
		void setTypeCycle(const std::vector<COLOR> &i_vCycle);
		COLOR get(void) const;
		operator COLOR() const;
		void set(COLOR i_eType);
		color operator =(COLOR i_eType);
		bool operator ==(const color & i_cRHO) const;
		bool operator ==(const COLOR & i_eRHO) const;
		color & operator++(int);
		color & operator--(int);
		color & operator++(void);
		color & operator--(void);
		color & operator+=(size_t i_tDelta);
		color & operator-=(size_t i_tDelta);
		color operator+(size_t i_tDelta) const;
		color operator-(size_t i_tDelta) const;
	};

	class stipple
	{
	private:
		size_t m_tCurr_Idx;
		std::vector<STIPPLE> m_vType_Cycle;
	public:
		stipple(void);
		void reset(void);
		std::vector<STIPPLE> getTypeCycle(void) const;
		void setTypeCycle(const std::vector<STIPPLE> &i_vCycle);
		STIPPLE get(void) const;
		operator STIPPLE() const;
		void set(STIPPLE i_eType);
		stipple operator =(STIPPLE i_eType);
		bool operator ==(const stipple & i_cRHO) const;
		bool operator ==(const STIPPLE & i_eRHO) const;
		stipple & operator++(int);
		stipple & operator--(int);
		stipple & operator++(void);
		stipple & operator--(void);
		stipple & operator+=(size_t i_tDelta);
		stipple & operator-=(size_t i_tDelta);
		stipple operator+(size_t i_tDelta) const;
		stipple operator-(size_t i_tDelta) const;
	};
	class eps_pair
	{
	public:
		double	m_dX;
		double	m_dY;

		eps_pair(void) {m_dX = m_dY = 0.0;}
		eps_pair(const double & i_dX, const double & i_dY) {m_dX = i_dX; m_dY = i_dY;}
		eps_pair operator +(const eps_pair & i_cRHO) const
		{
			return eps_pair(m_dX + i_cRHO.m_dX,m_dY + i_cRHO.m_dY);
		}
		eps_pair operator -(const eps_pair & i_cRHO) const
		{
			return eps_pair(m_dX + i_cRHO.m_dX,m_dY + i_cRHO.m_dY);
		}
		eps_pair & operator +=(const eps_pair & i_cRHO)
		{
			m_dX += i_cRHO.m_dX;
			m_dY += i_cRHO.m_dY;
			return *this;
		}
		eps_pair & operator -=(const eps_pair & i_cRHO)
		{
			m_dX -= i_cRHO.m_dX;
			m_dY -= i_cRHO.m_dY;
			return *this;
		}
	};
	class eps_triplet
	{
	public:
		double	m_dX;
		double	m_dY;
		double	m_dZ;

		eps_triplet(void) {m_dX = m_dY = m_dZ = 0.0;}
		eps_triplet(const double & i_dX, const double & i_dY, const double & i_dZ) {m_dX = i_dX; m_dY = i_dY; m_dZ = i_dZ;}
		eps_triplet operator +(const eps_triplet & i_cRHO) const
		{
			return eps_triplet(m_dX + i_cRHO.m_dX,m_dY + i_cRHO.m_dY,m_dZ + i_cRHO.m_dZ);
		}
		eps_triplet operator -(const eps_triplet & i_cRHO) const
		{
			return eps_triplet(m_dX - i_cRHO.m_dX,m_dY - i_cRHO.m_dY,m_dZ - i_cRHO.m_dZ);
		}
		eps_triplet & operator +=(const eps_triplet & i_cRHO)
		{
			m_dX += i_cRHO.m_dX;
			m_dY += i_cRHO.m_dY;
			m_dZ += i_cRHO.m_dZ;
			return *this;
		}
		eps_triplet & operator -=(const eps_triplet & i_cRHO)
		{
			m_dX -= i_cRHO.m_dX;
			m_dY -= i_cRHO.m_dY;
			m_dZ -= i_cRHO.m_dZ;
			return *this;
		}
	};

	class color_triplet
	{
	public:
		double		m_dRed; // 0.0 - 1.0
		double		m_dGreen; // 0.0 - 1.0
		double		m_dBlue; // 0.0 - 1.0

		color_triplet(void) {m_dRed = m_dGreen = m_dBlue = 0.0;}
		color_triplet(const double & i_dRed, const double & i_dGreen, const double & i_dBlue)
		{
			m_dRed = i_dRed;
			m_dGreen = i_dGreen;
			m_dBlue = i_dBlue;
		}
	};
	
	class page_parameters
	{
	public:
		unsigned int 	m_uiNum_Columns;
		unsigned int 	m_uiNum_Rows;
		double			m_dTitle_Margin;
		double			m_dLeft_Axis_Margin;
		double			m_dRight_Axis_Margin;
		double			m_dBottom_Axis_Margin;
		double			m_dTop_Axis_Margin;
		double			m_dWidth_Inches;
		double			m_dHeight_Inches;
		double			m_dSide_Unprintable_Margins_Inches;
		double			m_dTop_Bottom_Unprintable_Margins_Inches;
		double			m_dZ_Axis_Space;

		double			m_dTitle_Margin_Inches;
		double			m_dX_Axis_Margin_Inches;
		double			m_dY_Axis_Margin_Inches;
		double			m_dZ_Axis_Margin_Inches;

		bool			m_bLandscape;

		page_parameters(void)
		{
			m_dWidth_Inches = 8.5;
			m_dHeight_Inches = 11.0;
			m_bLandscape = true;
			m_uiNum_Columns = 1;
			m_uiNum_Rows = 1;

			m_dTitle_Margin = -1.0;//1.0 / m_dHeight_Inches;
			m_dTop_Axis_Margin = -1.0;//0.0;
			m_dBottom_Axis_Margin = -1.0;//1.2 / m_dHeight_Inches;
			m_dLeft_Axis_Margin = -1.0;//1.0 / m_dWidth_Inches;
			m_dRight_Axis_Margin = -1.0;//0.0;

			m_dTitle_Margin_Inches = -1.0; // value less than zero indicates the margin is determined by the plotting routines
			m_dX_Axis_Margin_Inches = -1.0; // value less than zero indicates the margin is determined by the plotting routines
			m_dY_Axis_Margin_Inches = -1.0; // value less than zero indicates the margin is determined by the plotting routines
			m_dZ_Axis_Margin_Inches = -1.0; // value less than zero indicates the margin is determined by the plotting routines


			m_dSide_Unprintable_Margins_Inches = 0.25; // Inches
			m_dTop_Bottom_Unprintable_Margins_Inches = 0.25; // Inches

//			m_bReformat = false;

		}

//		void Format_Journal_Column(void)
//		{ // format the page for a journal, typical width 3.35"
//			m_bReformat = true;
//		}

	};


	class axis_parameters
	{
	protected:

		void	Set_Defaults(void)
		{
			m_sTitle.clear();
			m_bLog = m_bInvert = false;
			m_dLower_Limit = nan("");
			m_dUpper_Limit = nan("");
			m_dLine_Width = 1.5;
			m_dMajor_Tick_Width = 2.0;
			m_dMinor_Tick_Width = 1.0;
			m_eColor = BLACK;
			m_eTitle_Color = BLACK;
			m_eMajor_Label_Color = BLACK;
			m_eMinor_Label_Color = BLACK;
			m_eMajor_Tick_Color = BLACK;
			m_eMinor_Tick_Color = BLACK;
			m_dMajor_Label_Size = 18.0; // Points
			m_dMinor_Label_Size = 18.0; // Points
			m_dTitle_Size = 36.0; // Points
			m_dMajor_Tick_Length = 20.0;
			m_dMinor_Tick_Length = 10.0;
			m_bLabel_Major_Indices = true;
			m_bLabel_Minor_Indices = false;
			m_sMajor_Index_Format = "m";
			m_sMinor_Index_Format = "";
			m_eScheme = rainbow;
			m_ctColor_Upper = color_triplet(1.,1.,1.);
			m_ctColor_Lower = color_triplet(0.,0.,0.);
			m_dBar_Width = 16.0;

		}
	public:
		std::string	m_sTitle;
		double	m_dLine_Width; // Points
		double	m_dMajor_Tick_Width; // Points
		double	m_dMajor_Tick_Length; // Points
		double	m_dMinor_Tick_Width; // Points
		double	m_dMinor_Tick_Length; // Points
		double	m_dMajor_Label_Size; // Points
		double	m_dMinor_Label_Size; // Points
		double	m_dTitle_Size; // Points
		COLOR	m_eColor; // color of line at edge of graph
		COLOR	m_eMajor_Label_Color;
		COLOR	m_eMinor_Label_Color;
		COLOR	m_eMajor_Tick_Color;
		COLOR	m_eMinor_Tick_Color;
		COLOR	m_eTitle_Color;
		std::string	m_sMajor_Index_Format;
		std::string	m_sMinor_Index_Format;
		bool	m_bLog;
		bool	m_bInvert;
		double	m_dLower_Limit; // note: use nan to indicate no limit
		double	m_dUpper_Limit; // note: use nan to indicate no limit
		bool	m_bLabel_Major_Indices;
		bool	m_bLabel_Minor_Indices; //@@TODO not implemented
		color_triplet	m_ctColor_Upper; // only used for Z axis
		color_triplet	m_ctColor_Lower; // only used for Z axis
		z_axis_scheme	m_eScheme; // only used for Z axis
		double	m_dBar_Width; /// Points, Z-axis only


		void	Set_Title(const char * i_lpszTitle)
		{
			if (i_lpszTitle != nullptr)
				m_sTitle = i_lpszTitle;
			else
				m_sTitle.clear();
		}
		const char * Get_Title(void) const {return m_sTitle.c_str();}
		std::string Get_Title_String(void) const {return m_sTitle;}
		axis_parameters(void)
		{
			Set_Defaults();
		}
		axis_parameters(const char * i_lpszAxis_Description)
		{ // simplify code by allowing a constructor with the only a title
			Set_Defaults();
			Set_Title(i_lpszAxis_Description);
		}
		axis_parameters(const char * i_lpszAxis_Description, bool i_bLog_Axis, bool i_bInvert_Axis, bool i_bSet_Min, const double & i_dMin, bool i_bSet_Max, const double & i_dMax)
		{ // simplify code by allowing a constructor with the most common parameters
			Set_Defaults();
			Set_Title(i_lpszAxis_Description);
			m_bLog = i_bLog_Axis;
			m_bInvert = i_bInvert_Axis;
			if (i_bSet_Min)
				m_dLower_Limit = i_dMin;
			if (i_bSet_Max)
				m_dUpper_Limit = i_dMax;
		}
	};

	class text_entity
	{
	public:
		enum entity_type {text,superscript,subscript,leftbrace,rightbrace,symbol};
		entity_type	m_eType;
		std::string	m_szData;
	};

	class epsfile
	{
	protected:
		char	m_lpszFormat[32];
		char	m_lpszSetDash[4];
		char	m_lpszMoveto[32];
		char	m_lpszLineto[32];
		char	m_lpszTranslate[32];
		char	m_lpszSetLineWidth[32];
		char	m_lpszSetRGBColor[32];
		char	m_lpszGsave[4];
		char	m_lpszGrestore[4];
		char	m_lpszStroke[4];
		char	m_lpszFill[4];
		char	m_lpszClosepath[4];
		char	m_lpszTxtCentered[16];
		char	m_lpszScalefontSetFont[8];
		char	m_lpszRectClip[32];

		char	* m_lpszFilename;
		FILE	* m_lpFileOut;
		class glyphsymbol : public std::string
		{
		protected:
			bool m_bRequires_Symbol_Font;
		public:
			glyphsymbol(void){m_bRequires_Symbol_Font = false;}
			glyphsymbol(const glyphsymbol & i_cRHO) : std::string(i_cRHO) {m_bRequires_Symbol_Font = i_cRHO.m_bRequires_Symbol_Font;}
			glyphsymbol(const std::string & i_szName, bool i_bRequires_Symbol_Font = false)  : std::string(i_szName) {m_bRequires_Symbol_Font = i_bRequires_Symbol_Font;}
			glyphsymbol(const char * i_szName, bool i_bRequires_Symbol_Font = false)  : std::string(i_szName)  {m_bRequires_Symbol_Font = i_bRequires_Symbol_Font;}

			bool requires_symbol(void)const{return m_bRequires_Symbol_Font;}
		};
//		std::map<std::string, std::string> m_mSymbol_Map;
		std::map<std::string, glyphsymbol> m_mSymbol_Map;
//		std::map<std::string, std::string> m_mSymbol_Equiv_Map;
		std::vector< text_entity > Parse_String(const std::string & i_szString) const;

	public:


		epsfile(const char * i_lpszFormat);
		~epsfile(void);

		void	Open_File(const char * i_lpszFilename, const char * i_lpszDocument_Title, const double & i_dWidth_Inches, const double & i_dHeight_Inches, bool i_bLandscape = true);
		void	Close_File(void);


		void Text(PS_FONT i_eFont, bool i_bItalic, bool i_bBold, int i_iFont_Size, PS_HORIZONTAL_JUSTIFICATION i_eHoirzontal_Justification, PS_VERTICAL_JUSTIFICATION i_eVertical_Justification, const color_triplet & i_cColor,const double & i_dX, const double & i_dY, const char * i_lpszText, const double & i_dRotation = 0.0, const double & i_dLine_Width = 1.0) const;
		void Rect_Clip(const double & i_dX, const double & i_dY, const double & i_dWidth, const double & i_dHeight) const;
		void Rect_Fill(const double & i_dX, const double & i_dY, const double & i_dWidth, const double & i_dHeight) const;
		void Move_To(const double & i_dX, const double & i_dY) const;
		void Line_To(const double & i_dX, const double & i_dY) const;
		void Translate(const double & i_dX, const double & i_dY) const;
		void State_Push(void) const;
		void State_Pop(void) const;
		void Set_Line_Width(const double &i_dLine_Width) const;
		void Set_RGB_Color(const color_triplet & i_cColor) const;
		void Set_RGB_Color(PS_STANDARD_COLORS i_eColor) const;
		void Stroke(void) const;
		void Fill(void) const;
		void Close_Path(void) const;
		void Comment(const char * i_lpszComment) const;
		void Set_Dash(const double * i_lpdPattern, unsigned int i_uiNum_Pattern_Elements, const double & i_dSpace) const;
		void Text_Bounding_Box(PS_FONT i_eFont, bool i_bItalic, bool i_bBold, int i_iFont_Size, PS_HORIZONTAL_JUSTIFICATION i_eHoirzontal_Justification, PS_VERTICAL_JUSTIFICATION i_eVertical_Justification, const color_triplet & i_cColor,const double & i_dX, const double & i_dY, const char * i_lpszText, const double & i_dRotation = 0.0, const double & i_dLine_Width = 1.0) const;
		void ColorImage(const double & i_dX, const double & i_dY, const double & i_dWidth, const double & i_dHeight, int i_iColor_Depth, const std::vector<color_triplet> & i_vctImage_Data, size_t i_nRow_Length) const;
	};


	class rectangle
	{
	public:
		double	m_dX_min;
		double	m_dX_max;
		double	m_dY_min;
		double	m_dY_max;

		rectangle(void)
		{
			m_dX_min = m_dX_max = m_dY_min = m_dY_max = 0.0;
		}
	};
	class line_parameters
	{
	public:
		COLOR		m_eColor;
		double		m_dWidth;
		STIPPLE		m_eStipple;

		line_parameters(void)
		{
			m_eColor = BLACK;
			m_dWidth = -1.0;
			m_eStipple = SOLID;
		}
	};
	
	class symbol_parameters
	{
	public:
		SYMBOL_TYPE	m_eType;
		double		m_dSize;
		COLOR		m_eColor;
		bool		m_bFilled;
		double		m_dLine_Width;
		symbol_parameters(void)
		{
			m_eColor = BLACK;
			m_dSize = 12.0;
			m_eType = SQUARE;
			m_bFilled = true;
			m_dLine_Width = -1.0;
		}
		void incrementTypeset(void)
		{
			size_t tType = ((int)m_eType - SQUARE);
			tType++;
			if (tType > (STAR6 - SQUARE))
				tType = 0;
			m_eType = (SYMBOL_TYPE)(SQUARE + tType);

			size_t tColor = ((int)m_eColor - BLACK);
			tColor++;
			if (tColor > (GREY_75 - BLACK))
				tColor = 0;
			m_eColor = (COLOR)(tColor + BLACK);

		}
		void decrementTypeset(void)
		{
			size_t tType = ((int)m_eType - SQUARE);
			tType--;
			if (tType > (STAR6 - SQUARE))
				tType = (STAR6 - SQUARE);
			m_eType = (SYMBOL_TYPE)(SQUARE + tType);

			size_t tColor = ((int)m_eColor - BLACK);
			tColor--;
			if (tColor > (GREY_75 - BLACK))
				tColor = (GREY_75 - BLACK);
			m_eColor = (COLOR)(tColor + BLACK);

		}
	};

	class text_parameters
	{
	public:
		PS_FONT m_eFont;
		bool m_bItalic;
		bool m_bBold;
		int m_iFont_Size;
		double m_dRotation;
		PS_HORIZONTAL_JUSTIFICATION m_eHorizontal_Justification;
		PS_VERTICAL_JUSTIFICATION m_eVertical_Justification;

		text_parameters(void)
		{
			m_eFont = TIMES;
			m_bItalic = false;
			m_bBold = false;
			m_iFont_Size = 18;
			m_dRotation = 0.0;
			m_eHorizontal_Justification = LEFT;
			m_eVertical_Justification = TOP;
		}
	};

	class errorbar_parameters
	{
	public:
		ERRORBAR_DIRECTION	m_eDirection;
		double				m_dTip_Width;
		ERRORBAR_TIP_TYPE	m_eTip_Type;
		unsigned int 		m_uiAssociated_Plot;

		errorbar_parameters(void)
		{
			m_eTip_Type = ERRORBAR_TIP_LINE;
			m_dTip_Width = 4.0;
			m_eDirection = ERRORBAR_X_LEFT;
			m_uiAssociated_Plot = -1;
		}
	};



	class legend_entry_parameters
	{
	public:
		bool m_bLine;
		bool m_bSymbol;

		line_parameters	m_cLine_Parameters;
		symbol_parameters	m_cSymbol_Parameters;

		std::string	m_szEntry_Text;

		legend_entry_parameters(void)
		{
			m_bLine = true;
			m_bSymbol = false;
			m_szEntry_Text.clear();
		}
	};

	class legend_parameters
	{
	public:
		bool			m_bFill;
		epsplot::COLOR 	m_eFill_Color;
		bool			m_bOutline;
		line_parameters	m_cOutline_Parameters;
		double		m_dX;
		double		m_dY;
		size_t		m_tNum_Col;
		size_t		m_tNum_Row;
		double		m_dColumn_Width;
		double		m_dRow_Height;
		double		m_dGap_Width;
		double		m_dLine_Length;

		text_parameters	m_cText_Parameters;

		legend_parameters(void)
		{
			m_bFill = false;
			m_bOutline = false;
			m_dX = -1;
			m_dY = -1;
			m_tNum_Col = 1;
			m_tNum_Row = -1;
			m_dColumn_Width = -1;
			m_dRow_Height = -1;
			m_dGap_Width = 12.0; // points
			m_dLine_Length = -1;//36.0; // points
			m_cText_Parameters.m_eVertical_Justification = MIDDLE; // override default of top
		}
	};

	enum item_type {type_line,type_symbol,type_rectangle,type_text,type_errorbar,type_3d};
	class	plot_item
	{
	public:
		item_type m_eType;

//		plot_item	*	m_lpPrev_Item;
//		plot_item	*	m_lpNext_Item;
		unsigned int 	m_uiPlot_Axes_To_Use[3];

		plot_item(item_type i_eType)
		{
			m_eType = i_eType;
//			m_lpPrev_Item = nullptr;
//			m_lpNext_Item = nullptr;
			m_uiPlot_Axes_To_Use[0] = 0;
			m_uiPlot_Axes_To_Use[1] = 0;
			m_uiPlot_Axes_To_Use[2] = -1;
		}
	};

	class plot_3d_item : public plot_item
	{
	public:

		eps_triplet * m_lppData;
		unsigned int m_uiNum_Points;
		z_axis_iterpolation_scheme	m_eInterpolation_Scheme;

		plot_3d_item(void) : plot_item(type_3d)
		{
			m_lppData = nullptr;
			m_uiNum_Points = 0;
			m_eInterpolation_Scheme = nearest;
		}
		~plot_3d_item(void)
		{
			if (m_lppData)
				delete [] m_lppData;
			m_lppData = nullptr;
			m_uiNum_Points = 0;
			m_eInterpolation_Scheme = nearest;
		}
	};
	class line_item : public plot_item
	{
	public:

		eps_pair * m_lppData;
		unsigned int m_uiNum_Points;
		line_parameters	m_cPlot_Line_Info;

		line_item(void) : plot_item(type_line), m_cPlot_Line_Info()
		{
			m_lppData = nullptr;
			m_uiNum_Points = 0;
		}
		~line_item(void)
		{
			if (m_lppData)
				delete [] m_lppData;
			m_lppData = nullptr;
			m_uiNum_Points = 0;
		}
	};

	class symbol_item : public plot_item
	{
	public:
		eps_pair * m_lppData;
		unsigned int m_uiNum_Points;
		symbol_parameters	m_cPlot_Symbol_Info;

		symbol_item(void) : plot_item(type_symbol), m_cPlot_Symbol_Info()
		{
			m_lppData = nullptr;
			m_uiNum_Points = 0;
		}
		~symbol_item(void)
		{
			if (m_lppData)
				delete [] m_lppData;
			m_lppData = nullptr;
			m_uiNum_Points = 0;
		}
	};

	class rectangle_item : public plot_item
	{
	public:
		rectangle	m_cPlot_Rectangle_Info;
		bool		m_bArea_Fill;
		COLOR		m_ePlot_Area_Fill_Color;
		bool		m_bDraw_Border;
		line_parameters	m_cPlot_Border_Info;

		rectangle_item(void) : plot_item(type_rectangle), m_cPlot_Rectangle_Info(), m_cPlot_Border_Info()
		{
			m_bArea_Fill = false;
			m_bDraw_Border = false;
			m_ePlot_Area_Fill_Color = BLACK;
		}
	};

	class text_item : public plot_item
	{
	protected:
		char * m_lpszText;
		size_t m_uiText_Alloc_Len;
	public:
		text_parameters	m_cText_Parameters;
		line_parameters	m_cLine_Parameters; // stipple is ignored
		double m_dX;
		double m_dY;

		text_item(void) : plot_item(type_text), m_cText_Parameters(), m_cLine_Parameters()
		{
			m_dX = 0.0;
			m_dY = 0.0;
			m_lpszText = nullptr;
			m_uiText_Alloc_Len = 0;
		}
		~text_item(void)
		{
			if (m_lpszText)
				delete [] m_lpszText;
			m_lpszText = nullptr;
			m_uiText_Alloc_Len = 0;
		}
		void Set_Text(const char * i_lpszText)
		{
			size_t uiLen = strlen(i_lpszText) + 1;
			if (uiLen > m_uiText_Alloc_Len)
			{
				if (m_lpszText)
					delete [] m_lpszText;
				m_uiText_Alloc_Len = uiLen;
				m_lpszText = new char[m_uiText_Alloc_Len];
			}
			if (m_lpszText)
				strcpy(m_lpszText,i_lpszText);
		}
		const char * Get_Text(void) const {return m_lpszText;}
	};

	class errorbar_item : public plot_item
	{
	public:
		errorbar_parameters	m_cErrorbar_Info;
		double * m_lppData;
		unsigned int m_uiNum_Points;
		line_parameters	m_cPlot_Line_Info;

		errorbar_item(void) : plot_item(type_errorbar), m_cPlot_Line_Info(), m_cErrorbar_Info()
		{
			m_lppData = nullptr;
			m_uiNum_Points = 0;
		}
		~errorbar_item(void)
		{
			if (m_lppData)
				delete [] m_lppData;
			m_lppData = nullptr;
			m_uiNum_Points = 0;
		}
	};



	class axis_metadata
	{
	public:
		unsigned int	m_uiIdentifier;
		bool			m_bEnabled;
		axis_parameters	m_cParameters;
		double			m_dRange; // generated and used at plot time
		double			m_dScale; // generated and used at plot time
		double			m_dLower_Limit; // generated and used at plot time
		double			m_dUpper_Limit; // generated and used at plot time
		double			m_dStart; // generated and used at plot time
		double			m_dEnd; // generated and used at plot time

		void Reset_Limits(void)
		{
			m_dScale = m_dRange = m_dUpper_Limit = m_dLower_Limit = nan("");
		}
		axis_metadata(void) : m_cParameters()
		{
			m_bEnabled = true;
			m_uiIdentifier = -1;
			Reset_Limits();
		}
		axis_metadata(const axis_parameters & i_cAxis_Parameters) : m_cParameters(i_cAxis_Parameters)
		{
			m_bEnabled = true;
			m_uiIdentifier = -1;
			Reset_Limits();
		}

		void Adjust_Limits(const double & i_dValue)
		{
			if (std::isnan(m_cParameters.m_dLower_Limit) && (std::isnan(m_dLower_Limit) || m_dLower_Limit > i_dValue))
				m_dLower_Limit = i_dValue;
			if (std::isnan(m_cParameters.m_dUpper_Limit) && (std::isnan(m_dUpper_Limit) || m_dUpper_Limit < i_dValue))
				m_dUpper_Limit = i_dValue;
		}
		void Finalize_Limit(void)
		{
			if (!std::isnan(m_cParameters.m_dLower_Limit))
				m_dLower_Limit = m_cParameters.m_dLower_Limit;
			if (!std::isnan(m_cParameters.m_dUpper_Limit))
				m_dUpper_Limit = m_cParameters.m_dUpper_Limit;
			m_dRange = m_dUpper_Limit - m_dLower_Limit;

			if (m_cParameters.m_bLog && m_cParameters.m_bInvert)
			{
				if (m_dUpper_Limit > 0.0 && m_dLower_Limit > 0.0)
				{
					m_dStart = log10(m_dUpper_Limit);
					m_dEnd = log10(m_dLower_Limit);
				}
			}
			else if (m_cParameters.m_bLog)
			{
				if (m_dUpper_Limit > 0.0 && m_dLower_Limit > 0.0)
				{
					m_dStart = log10(m_dLower_Limit);
					m_dEnd = log10(m_dUpper_Limit);
				}
			}
			else if (m_cParameters.m_bInvert)
			{
				m_dStart = m_dUpper_Limit;
				m_dEnd = m_dLower_Limit;
			}
			else
			{
				m_dStart = m_dLower_Limit;
				m_dEnd = m_dUpper_Limit;
			}
		}
		void Set_Scale(const double & i_dGraph_Space)
		{
			if (m_dRange > 0.0)
				m_dScale = i_dGraph_Space / m_dRange;
			else
				m_dScale = 1.0;
			if (m_cParameters.m_bLog)
				m_dScale = i_dGraph_Space / fabs(m_dEnd - m_dStart);


			if (m_cParameters.m_bInvert)
			{
				m_dScale *= -1.0;
			}
		}
		double Scale(const double &i_dX) const
		{
			double dX = nan("");
			if (m_cParameters.m_bLog && i_dX > 0.0)
				dX = log10(i_dX);
			else if (!m_cParameters.m_bLog)
				dX = i_dX;
			dX -= m_dStart;
			dX *= m_dScale;
			return dX;
		}
		double Reverse_Scale(const double &i_dX) const
		{
			double dX = i_dX / m_dScale;
			dX += m_dStart;
			if (m_cParameters.m_bLog)
				dX = std::pow(10.0,dX);
			return dX;
		}


		color_triplet	Get_Color(const double & i_dX) // only relevant for Z axis
		{
			color_triplet	cRet;
			double dVal = i_dX;
			if (m_cParameters.m_bLog && i_dX > 0.0)
				dVal = log10(i_dX);
			dVal -= m_dStart;
			dVal /= (m_dEnd - m_dStart);
			if (dVal < 0.0)
				dVal = 0.0;
			else if (dVal > 1.0)
				dVal = 1.0;
			//printf("%f %f %f %f ",i_dX,m_dStart, m_dEnd, dVal);
			switch (m_cParameters.m_eScheme)
			{
			case inverse_rainbow:
				dVal = 1.0 - dVal;
			case rainbow:
				if (dVal > 0.833333333333333333)
				{
					m_cParameters.m_ctColor_Lower = color_triplet(0.0,1.0,1.0);
					m_cParameters.m_ctColor_Upper = color_triplet(0.2,0.0,1.0);
					dVal -= 0.833333333333333333;
				}
				else if (dVal > 0.6666666666666666666)
				{
					m_cParameters.m_ctColor_Lower = color_triplet(0.0,0.0,1.0);
					m_cParameters.m_ctColor_Upper = color_triplet(0.0,1.0,1.0);
					dVal -= 0.66666666666666666;
				}
				else if (dVal > 0.5)
				{
					m_cParameters.m_ctColor_Lower = color_triplet(0.0,1.0,0.0);
					m_cParameters.m_ctColor_Upper = color_triplet(0.0,0.0,1.0);
					dVal -= 0.5;
				}
				else if (dVal > 0.3333333333333333333)
				{
					m_cParameters.m_ctColor_Lower = color_triplet(1.0,1.0,0.0);
					m_cParameters.m_ctColor_Upper = color_triplet(0.0,1.0,0.0);
					dVal -= 0.3333333333333333333;
				}
				else if (dVal > 0.1666666666666666666)
				{
					m_cParameters.m_ctColor_Lower = color_triplet(1.0,0.5,0.0);
					m_cParameters.m_ctColor_Upper = color_triplet(1.0,1.0,0.0);
					dVal -= 0.1666666666666666666;
				}
				else
				{
					m_cParameters.m_ctColor_Lower = color_triplet(1.0,0.0,0.0);
					m_cParameters.m_ctColor_Upper = color_triplet(1.0,0.5,0.0);
				}
				dVal /= 0.166666666666666667;
				//printf("%f (%f %f %f) - (%f %f %f)\n",dVal,m_cParameters.m_ctColor_Lower.m_dRed,m_cParameters.m_ctColor_Lower.m_dGreen,m_cParameters.m_ctColor_Lower.m_dBlue,m_cParameters.m_ctColor_Upper.m_dRed,m_cParameters.m_ctColor_Upper.m_dGreen,m_cParameters.m_ctColor_Upper.m_dBlue);
				break;
			}
			cRet.m_dRed = m_cParameters.m_ctColor_Lower.m_dRed + (m_cParameters.m_ctColor_Upper.m_dRed * dVal - m_cParameters.m_ctColor_Lower.m_dRed * dVal);
			cRet.m_dGreen = m_cParameters.m_ctColor_Lower.m_dGreen + (m_cParameters.m_ctColor_Upper.m_dGreen * dVal - m_cParameters.m_ctColor_Lower.m_dGreen * dVal);
			cRet.m_dBlue = m_cParameters.m_ctColor_Lower.m_dBlue + (m_cParameters.m_ctColor_Upper.m_dBlue * dVal - m_cParameters.m_ctColor_Lower.m_dBlue * dVal);
			//printf("(%f %f %f)\n",cRet.m_dRed,cRet.m_dGreen,cRet.m_dBlue);
			return cRet;
		}
	};
		

	class data
	{
	protected:
		std::string m_szTitle;
		double	m_dTitle_Size;
		COLOR	m_eTitle_Color;
		std::string m_szFilename;

		std::vector<plot_item *> m_vcPlot_Item_List;

		inline plot_item * Get_Plot_Item(unsigned int i_uiID) const
		{
			//printf("GPI: %i (%i)\n",i_uiID,m_vcPlot_Item_List.size());
			plot_item * plRet = nullptr;
			if (i_uiID < m_vcPlot_Item_List.size())
				plRet = m_vcPlot_Item_List[i_uiID];
			return plRet;
		}

		color_triplet	m_cCustom_Colors[16];
		double	* m_lpdCustom_Stipple[16];
		unsigned int m_uiStipple_Length[16];

		std::vector<axis_metadata>	m_cX_Axis_Parameters;
		std::vector<axis_metadata>	m_cY_Axis_Parameters;
		std::vector<axis_metadata>	m_cZ_Axis_Parameters;

		std::map<size_t,legend_parameters>	m_cLegends;
		std::map<size_t,legend_entry_parameters>	m_cLegend_Entries;
		std::map<size_t,size_t>	m_cLegend_Entry_Crossref;

		double Estimate_Text_Width(const std::string &i_sString,const epsplot::text_parameters & i_cText_Param) const;
		double Estimate_Text_Height(const std::string &i_sString,const epsplot::text_parameters & i_cText_Param) const;

		legend_parameters Preprocess_Legend(unsigned int i_uiLegend_ID) const;
		std::vector<size_t> Get_Legend_Entries(unsigned int i_uiLegend_ID) const;


		void Draw_Symbol(epsfile & io_cEPS, const double & i_dX, const double & i_dY, const symbol_parameters & i_cSymbol_Param);
		void	Deallocate_Plot_Data(void)
		{
			for (std::vector<plot_item *>::iterator	cIterator = m_vcPlot_Item_List.begin(); cIterator != m_vcPlot_Item_List.end(); cIterator++)
			{
				line_item * lpcLine = nullptr;
				symbol_item * lpcSymbol = nullptr;
				rectangle_item * lpcRectangle = nullptr;
				text_item * lpcText = nullptr;
				plot_3d_item * lpcPlot_3d = nullptr;
				errorbar_item * lpcErrorbar = nullptr;
				plot_item * lpCurr = *cIterator;

				switch (lpCurr->m_eType)
				{
				case type_3d:
					lpcPlot_3d = (plot_3d_item *) lpCurr;
					break;
				case type_line:
					lpcLine = (line_item *) lpCurr;
					break;
				case type_symbol:
					lpcSymbol = (symbol_item *) lpCurr;
					break;
				case type_rectangle:
					lpcRectangle = (rectangle_item *) lpCurr;
					break;
				case type_text:
					lpcText = (text_item *) lpCurr;
					break;
				case type_errorbar:
					lpcErrorbar = (errorbar_item *) lpCurr;
					break;
				}
				if (lpcLine)
					delete lpcLine;
				else if (lpcSymbol)
					delete lpcSymbol;
				else if (lpcRectangle)
					delete lpcRectangle;
				else if (lpcText)
					delete lpcText;
				else if (lpcPlot_3d)
					delete lpcPlot_3d;
				else if (lpcErrorbar)
					delete lpcErrorbar;
			}
			m_vcPlot_Item_List.clear();
			m_cLegends.clear();
			m_cLegend_Entries.clear();
			m_cLegend_Entry_Crossref.clear();

		}
		void	Deallocate_X_Axis_Data(void)
		{
			m_cX_Axis_Parameters.clear();
		}
		void	Deallocate_Y_Axis_Data(void)
		{
			m_cY_Axis_Parameters.clear();
		}
		void	Deallocate_Z_Axis_Data(void)
		{
			m_cZ_Axis_Parameters.clear();
		}
		void	Deallocate_Axis_Data(void)
		{
			Deallocate_X_Axis_Data();
			Deallocate_Y_Axis_Data();
			Deallocate_Z_Axis_Data();
		}
		void	Null_Pointers(void)
		{
			m_vcPlot_Item_List.clear();
			m_szTitle.clear();
			m_szFilename.clear();

			for (unsigned int uiI = 0; uiI < 16; uiI++)
			{
				m_lpdCustom_Stipple[uiI] = nullptr;
				m_uiStipple_Length[uiI] = 0;
			}
		}
		std::vector<axis_metadata> * Get_Axis_Metedata_Vector_Ptr(AXIS i_eAxis);
		const std::vector<axis_metadata> * Get_Axis_Metedata_Vector_Ptr_Const(AXIS i_eAxis) const;


	public:
	// Methods for the title
		void	Set_Plot_Title(const char * i_lpszTitle, const double & i_dSize = 36.0, COLOR i_eColor = BLACK) { if (i_lpszTitle == nullptr) m_szTitle.clear(); else m_szTitle = i_lpszTitle;}
		const char *	Get_Plot_Title(void) const { return m_szTitle.c_str();}

	// Methods for the destination file
		void	Set_Plot_Filename(const char * i_lpszFilename) { if(i_lpszFilename == nullptr) m_szFilename.clear(); else m_szFilename = i_lpszFilename;}
		const char *	Get_Plot_Filename(void) const { return m_szFilename.c_str();}


	// Methods for setting data to be plotted
		///////////////////// Plots //////////////////////////////
		     ////////////////// Set //////////////////
		unsigned int	Set_Plot_Data(const double * i_lpdX_Values, const double * i_lpdY_Values, unsigned int i_uiNum_Points, COLOR i_eColor, STIPPLE i_eStipple, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, const double & i_dLine_Width);
		unsigned int	Set_Plot_Data(const double * i_lpdX_Values, const double * i_lpdY_Values, unsigned int i_uiNum_Points, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Set_Plot_Data(const std::vector<double> &i_vdX_Values, const std::vector<double> &i_vdY_Values, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Set_Plot_Data(const std::vector<eps_pair> &i_vpValues, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		     //////////////// Modify /////////////////
		unsigned int	Modify_Plot_Data(unsigned int i_uiPlot_Data_ID, const double * i_lpdX_Values, const double * i_lpdY_Values, unsigned int i_uiNum_Points, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Modify_Plot_Data(unsigned int i_uiPlot_Data_ID, const std::vector<double> &i_vdX_Values, const std::vector<double> &i_vdY_Values, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Modify_Plot_Data(unsigned int i_uiPlot_Data_ID, const std::vector<eps_pair> &i_vpValues, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);


		///////////////////// 3d Plots //////////////////////////////
		     ////////////////// Set //////////////////
		unsigned int	Set_Plot_Data(const double * i_lpdX_Values, const double * i_lpdY_Values, const double * i_lpdZ_Values, unsigned int i_uiNum_Points, z_axis_iterpolation_scheme i_eInterpolation_Scheme, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, unsigned int i_uiZ_Axis_ID);
		unsigned int	Set_Plot_Data(const std::vector<double> &i_vdX_Values, const std::vector<double> &i_vdY_Values, const std::vector<double> &i_vdZ_Values, z_axis_iterpolation_scheme i_eInterpolation_Scheme, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, unsigned int i_uiZ_Axis_ID);
		unsigned int	Set_Plot_Data(const std::vector<eps_triplet> &i_vpValues, z_axis_iterpolation_scheme i_eInterpolation_Scheme, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, unsigned int i_uiZ_Axis_ID);
		     //////////////// Modify /////////////////
		unsigned int	Modify_Plot_Data(unsigned int i_uiPlot_Data_ID, const double * i_lpdX_Values, const double * i_lpdY_Values, const double * i_lpdZ_Values, unsigned int i_uiNum_Points, z_axis_iterpolation_scheme i_eInterpolation_Scheme, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, unsigned int i_uiZ_Axis_ID);
		unsigned int	Modify_Plot_Data(unsigned int i_uiPlot_Data_ID, const std::vector<double> &i_vdX_Values, const std::vector<double> &i_vdY_Values, const std::vector<double> &i_vdZ_Values, z_axis_iterpolation_scheme i_eInterpolation_Scheme, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, unsigned int i_uiZ_Axis_ID);
		unsigned int	Modify_Plot_Data(unsigned int i_uiPlot_Data_ID, const std::vector<eps_triplet> &i_vpValues, z_axis_iterpolation_scheme i_eInterpolation_Scheme, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID, unsigned int i_uiZ_Axis_ID);

		///////////////////// Errorbars //////////////////////////////
		     ////////////////// Set //////////////////
		unsigned int	Set_Errorbar_Data(const errorbar_parameters & i_cErrorbar_Parameters, const std::vector<double> &i_vdValues, const line_parameters & i_cLine_Parameters);
		unsigned int	Modify_Errorbar_Data(unsigned int i_uiPlot_Data_ID, const errorbar_parameters & i_cErrorbar_Parameters, const std::vector<double> &i_vdValues, const line_parameters & i_cLine_Parameters);

		///////////////////// Symbols //////////////////////////////
		     ////////////////// Set //////////////////
		unsigned int	Set_Symbol_Data(const double * i_lpdX_Values, const double * i_lpdY_Values, unsigned int i_uiNum_Points, const symbol_parameters & i_cSymbol_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Set_Symbol_Data(const std::vector<double> &i_vdX_Values, const std::vector<double> &i_vdY_Values, const symbol_parameters & i_cSymbol_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Set_Symbol_Data(const std::vector<eps_pair> &i_vpValues, const symbol_parameters & i_cSymbol_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		     //////////////// Modify /////////////////
		unsigned int	Modify_Symbol_Data(unsigned int i_uiPlot_Data_ID, const double * i_lpdX_Values, const double * i_lpdY_Values, unsigned int i_uiNum_Points, const symbol_parameters & i_cSymbol_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Modify_Symbol_Data(unsigned int i_uiPlot_Data_ID, const std::vector<double> &i_vdX_Values, const std::vector<double> &i_vdY_Values, const symbol_parameters & i_cSymbol_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);
		unsigned int	Modify_Symbol_Data(unsigned int i_uiPlot_Data_ID, const std::vector<eps_pair> &i_vpValues, const symbol_parameters & i_cSymbol_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);

	// Methods for controlling axes
		///////////////////// Axis control //////////////////////////////
		     ////////////////// Set //////////////////
		unsigned int	Set_X_Axis_Parameters(const char * i_lpszAxis_Title, bool i_bLog_Axis, bool i_bInvert_Axis, bool i_bSet_Min, const double & i_dMin, bool i_bSet_Max, const double & i_dMax);
		unsigned int	Set_Y_Axis_Parameters(const char * i_lpszAxis_Title, bool i_bLog_Axis, bool i_bInvert_Axis, bool i_bSet_Min, const double & i_dMin, bool i_bSet_Max, const double & i_dMax);
		unsigned int	Set_Axis_Parameters(AXIS i_eAxis, const axis_parameters & i_cAxis_Parameters);
		unsigned int	Set_Z_Axis_Parameters(const axis_parameters & i_cAxis_Parameters);
		unsigned int	Set_X_Axis_Parameters(const axis_parameters & i_cAxis_Parameters);
		unsigned int	Set_Y_Axis_Parameters(const axis_parameters & i_cAxis_Parameters);
		     //////////////// Modify /////////////////
		unsigned int	Modify_Axis_Parameters(AXIS i_eAxis, unsigned int i_uiWhich, const axis_parameters & i_cAxis_Parameters);
		unsigned int	Modify_X_Axis_Parameters(unsigned int i_uiWhich, const axis_parameters & i_cAxis_Parameters);
		unsigned int	Modify_Y_Axis_Parameters(unsigned int i_uiWhich, const axis_parameters & i_cAxis_Parameters);
		unsigned int	Modify_Z_Axis_Parameters(unsigned int i_uiWhich, const axis_parameters & i_cAxis_Parameters);
		     ////////////////// Get //////////////////
		axis_parameters	Get_Axis_Parameters(AXIS i_eAxis, unsigned int i_uiWhich)  const;
		axis_parameters	Get_X_Axis_Parameters(unsigned int i_uiWhich)  const;
		axis_parameters	Get_Y_Axis_Parameters(unsigned int i_uiWhich)  const;
		axis_parameters	Get_Z_Axis_Parameters(unsigned int i_uiWhich)  const;
		unsigned int	Get_Num_Axes(AXIS i_eAxis) const
		{
			unsigned int uiRet = 0;
			switch (i_eAxis)
			{
			case X_AXIS:
				uiRet = m_cX_Axis_Parameters.size();
				break;
			case Y_AXIS:
				uiRet = m_cY_Axis_Parameters.size();
				break;
			case Z_AXIS:
				uiRet = m_cZ_Axis_Parameters.size();
				break;
			}
			return uiRet;
		}
		unsigned int	Get_Num_X_Axes(void) const {return Get_Num_Axes(X_AXIS);};
		unsigned int	Get_Num_Y_Axes(void) const {return Get_Num_Axes(Y_AXIS);};
		unsigned int	Get_Num_Z_Axes(void) const {return Get_Num_Axes(Z_AXIS);};

	// Methods for colors
		void	Define_Custom_Color(COLOR i_eColor, const color_triplet & i_cColor);
		color_triplet Get_Color(COLOR i_eColor) const;

	// Methods for stipples
		void	Define_Custom_Stipple(STIPPLE i_eStipple, const double * i_lpdStipple, unsigned int i_uiStipple_Length);
		const double *	Get_Stipple(STIPPLE i_eStipple, unsigned int & o_uiStipple_Length) const;

	// Methods for rectangles
		unsigned int	Set_Rectangle_Data(const rectangle & i_cArea, bool i_bFill, COLOR i_eFill_Color, bool i_bBorder, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_Type, unsigned int i_uiY_Axis_Type);
		unsigned int	Modify_Rectangle_Data(unsigned int i_uiPlot_Data_ID, const rectangle & i_cArea, bool i_bFill, COLOR i_eFill_Color, bool i_bBorder, const line_parameters & i_cLine_Parameters, unsigned int i_uiX_Axis_ID, unsigned int i_uiY_Axis_ID);

	// Methods for text
		unsigned int	Set_Text_Data(const double & i_dX, const double & i_dY, const char * i_lpszText, const line_parameters & i_cLine_Parameters, const text_parameters & i_cText_Parameters, unsigned int i_uiX_Axis_Type, unsigned int i_uiY_Axis_Type);
		unsigned int	Modify_Text_Data(unsigned int i_uiText_Data_ID, const double & i_dX, const double & i_dY, const char * i_lpszText, const line_parameters & i_cLine_Parameters, const text_parameters & i_cText_Parameters, unsigned int i_uiX_Axis_Type, unsigned int i_uiY_Axis_Type);

	// Methods for legend
		unsigned int	Set_Legend(const legend_parameters & i_cLegend_Parameters);
		unsigned int	Modify_Legend(unsigned int i_uiLegend_ID, const legend_parameters & i_cLegend_Parameters);

		unsigned int	Set_Legend_Entry(unsigned int i_uiLegend_ID, const legend_entry_parameters & i_cLegend_Parameters);
		unsigned int	Modify_Legend_Entry(unsigned int i_uiLegend_Entry_ID, unsigned int i_uiLegend_ID, const legend_entry_parameters & i_cLegend_Parameters);
	// Methods for plotting
		void	Plot(const page_parameters & i_cGrid);


	// Methods for emptying the plot
		void	Clear_Plots(void)	{Deallocate_Plot_Data();}

		data(const data & i_cRHO)
		{
			throw(1); // copy constructor not allowed for now
		}
		data & operator = (const data & i_cRHO)
		{
			throw(1);
		}
		data(void)
		{
			Null_Pointers();
		}
		~data(void)
		{
			Deallocate_Plot_Data();

			for (unsigned int uiI = 0; uiI < 16; uiI++)
			{
				if (m_lpdCustom_Stipple[uiI])
					delete [] m_lpdCustom_Stipple[uiI];
				m_uiStipple_Length[uiI] = 0;
			}

			Deallocate_Axis_Data();

			Null_Pointers();
		}
	};


	extern const color_triplet	g_cColor_Std_Black;
	extern const color_triplet	g_cColor_Std_Red;
	extern const color_triplet	g_cColor_Std_Green;
	extern const color_triplet	g_cColor_Std_Blue;
	extern const color_triplet	g_cColor_Std_Cyan;
	extern const color_triplet	g_cColor_Std_Magenta;
	extern const color_triplet	g_cColor_Std_Yellow;
	extern const color_triplet	g_cColor_Std_Grey_25;
	extern const color_triplet	g_cColor_Std_Grey_50;
	extern const color_triplet	g_cColor_Std_Grey_75;
	extern const color_triplet	g_cColor_Std_White;

};



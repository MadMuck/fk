GO
/****** Object:  StoredProcedure [dbo].[SP_UpdateUserDetail]    Script Date: 12/06/2017 17:25:11 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO




-- =============================================
-- Author:		Jian Guankun
-- Create date: 2011-11-8
-- Description:	更新用户信息
-- =============================================
ALTER PROCEDURE [dbo].[SP_UpdateUserDetail]
	@UserID				INT, --用户ID
	@UpdateType			INT, --更改类型，1为基本资料，2为详细资料
	@UserPassword		NVARCHAR(50),
	@nickName			NVARCHAR(20),
	@Sex				INT,
	@SignDescr			NVARCHAR(128),	--个性签名
	@PhoneNum			VARCHAR(50)='',	--移动电话号码
	@RealName			VARCHAR(20)='',	--真实姓名
	@ZJ_Number			VARCHAR(50)='',	--证件号码
	@QQNum				VARCHAR(20)='',	--QQ号码
	@AdrNation			VARCHAR(50)='',	--国籍
	@AdrProvince		VARCHAR(50)='',	--省份
	@AdrCity			VARCHAR(50)='',	--城市
	@ZipCode			VARCHAR(10)='',	--邮政编码
	@Email				VARCHAR(50)='',	--电子邮件
	@Address			NVARCHAR(500)='',	--联系地址
	@Schooling			NVARCHAR(20)='',	--文化程度
	@HomePage			VARCHAR(128)='',	--主页
	@TelNo 				VARCHAR(20)='',	--固定电话
	@MSNID 				NVARCHAR(50)=''	--MSN帐号
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @UserIDSel INT
	DECLARE @UserPasswordCurr	NVARCHAR(50)
	DECLARE @Salt				VARCHAR(20)

	--检查有没有用户@UserID
	SELECT @UserIDSel = u.UserID,@UserPasswordCurr = u.Pass,@Salt = Salt FROM TUsers u WHERE u.UserID = @UserID

	IF @UserIDSel IS NULL 
	BEGIN
		RETURN 0	--用户不存在
	END

	--如果存在Salt，则采用另一种加密方式
	IF @Salt IS NOT NULL AND DATALENGTH(LTRIM(@Salt)) > 0
		BEGIN
			DECLARE @MD5Temp VARCHAR(50) --NVARCHAR因为是Unicode编码，MD5后结果不一样
			SET @MD5Temp = @UserPassword
			SET @MD5Temp = sys.fn_VarBinToHexStr(HashBytes('MD5', @MD5Temp + @Salt))
			SET @MD5Temp = substring(@MD5Temp,charindex('x',@MD5Temp)+1,len(@MD5Temp))
			IF @MD5Temp <> @UserPasswordCurr
				RETURN 0
		END
	ELSE IF @UserPassword <> @UserPasswordCurr
		BEGIN
			RETURN 0
		END

	DECLARE	@SQL NVARCHAR(500)

	--更新TUser表
	SET @SQL ='UPDATE TUsers SET '
	IF @UpdateType = 1
	BEGIN
		SET @SQL = @SQL
				   + 'NickName = ''' + @nickName + ''','
				   + 'Sex ='		 + CAST(@Sex AS CHAR(1)) + ' '
		SET @SQL = @SQL + 'WHERE UserID = ' + CAST(@UserIDSel AS VARCHAR(20))
		EXEC(@SQL)
	END

	--更新TUserInfo表
	SET @SQL ='UPDATE TUserInfo SET '
	IF @UpdateType = 2
	BEGIN
		SET @SQL = @SQL
				   + 'AdrNation = '''	+ @AdrNation + ''','
				   + 'AdrProvince = '''	+ @AdrProvince + ''','
				   + 'AdrCity = '''		+ @AdrCity + ''' '
		SET @SQL = @SQL + 'WHERE UserID = ' + CAST(@UserIDSel AS VARCHAR(20))
		EXEC(@SQL)
	END

	--如果Web_Users没有用户@UserID的记录，添加一条
	IF NOT EXISTS(SELECT UserID FROM Web_Users WHERE UserID = @UserID)
	BEGIN
		INSERT Web_Users(UserID) VALUES(@UserID)
	END

	--更新Web_Users表
	SET @SQL ='UPDATE Web_Users SET '
	IF @UpdateType = 1
	BEGIN
		SET @SQL = @SQL
				   + 'SignDescr = '''	+ @SignDescr + ''' '
	END
	ELSE IF @UpdateType = 2
	BEGIN
		SET @SQL = @SQL
				   + 'RealName = '''	+ @RealName + ''','
				   + 'ZJ_Number = '''	+ @ZJ_Number + ''','
				   + 'Phone = '''		+ @PhoneNum + ''','
				   + 'QQNum = '''		+ @QQNum + ''','
				   + 'ZipCode = '''		+ @ZipCode + ''','
				   + 'Email = '''		+ @Email + ''','
				   + 'Address = '''		+ @Address + ''','
				   + 'Schooling = '''	+ @Schooling + ''','
				   + 'HomePage = '''	+ @HomePage + ''','
				   + 'TelNo = '''		+ @TelNo + ''','
				   + 'MSNID = '''		+ @MSNID + ''' '
	END
	
	SET @SQL = @SQL + 'WHERE UserID = ' + CAST(@UserIDSel AS VARCHAR(20))
	EXEC(@SQL)

	RETURN 1	--成功

END





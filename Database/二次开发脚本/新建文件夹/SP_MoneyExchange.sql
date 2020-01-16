
-- ================================================
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		LiMu
-- Create date: 2019-03-20
-- Description:	���ʹ�ý�Ҷһ����꣨���긴�ý�ȯ�ֶΣ�
-- ���ڱ�����������  �ʹ��߻� �������С�������  ���磨1����Ҷһ�1.5������ ���� 1.5����Ҷһ�1�����꣩ ����С��û���ж�
-- =============================================
CREATE PROCEDURE [dbo].[SP_MoneyExchange]
(
	@UserID INT ,			-- ���ID
    @ExchangeNum INT		-- ��Ҫ�һ���������
)	
AS
BEGIN
	SET NOCOUNT ON;
	declare @Moneybegin bigint			--��Ҷһ�֮ǰ���Ͻ������
	declare @CrushingDrillBegin bigint	--��Ҷһ�֮ǰ�������꣨��ȯ������
	declare @Money bigint				--��Ҷһ�֮�����Ͻ������
	declare @CrushingDrill bigint		--��Ҷһ�֮���������꣨��ȯ������
	
	DECLARE @UserName NVARCHAR(64)
	SELECT @UserName=UserName from dbo.TUsers WHERE UserID=@UserID
	
    select @Moneybegin=WalletMoney from TUserInfo where UserID=@UserID
    select @CrushingDrillBegin=Lotteries from Web_Users where UserID=@UserID
    declare @CrushingDrillForGold int    --���꣨��ȯ���һ����
    declare @GoldForCrushingDrill int    --��Ҷһ����꣨��ȯ��
    set @CrushingDrillForGold=0
    set @GoldForCrushingDrill=0
    
    select @CrushingDrillForGold=CrushingDrillForGold,@GoldForCrushingDrill=GoldForCrushingDrill
    from Web_Config
    if @CrushingDrillForGold<1 or @GoldForCrushingDrill<1
    begin
		return 1         --�һ��������ô���
	end	
	
	
	declare @ConsumptionOfGold bigint   --�һ�������Ҫ�����ĵĽ������
	
    if @CrushingDrillForGold=1 --��ʾ�����ң�����1��1���һ�1�����꣨��ȯ��������վФ�����飩
    begin
		
		set @ConsumptionOfGold=@GoldForCrushingDrill*@ExchangeNum --�һ����꣨��ȯ����Ҫ�����ĵĽ������
		if @ConsumptionOfGold>@Moneybegin
		begin
			return 2     --������Ͻ�Ҳ���
		end
		--�һ�   ����������ϵĽ����������������
		update TUserInfo set WalletMoney=WalletMoney-@ConsumptionOfGold where UserID=@UserID
		update Web_Users set Lotteries=Lotteries+@ExchangeNum where UserID=@UserID
		select @Money=WalletMoney from TUserInfo where UserID=@UserID
		select @CrushingDrill=Lotteries from Web_Users where UserID=@UserID
		
		---����仯��־��
		INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
			VALUES  (@UserID, @UserName ,@CrushingDrillBegin ,@ExchangeNum ,@CrushingDrill,GETDATE(),'��ҽ�Ҷһ�����',6)
		
		select @Money as Money,@CrushingDrill as CrushingDrill
		return 0         --�ɹ�
    end
    else    --����1����Ҷһ������������
    begin
		if(cast(@ExchangeNum as float)/@CrushingDrillForGold)<>(cast(@ExchangeNum/@CrushingDrillForGold as int))--��Ҫ�һ�������������Ǻ�̨���öһ�����������������
		return 1         --�һ��������ô���																		--  ������Ҫ��42��  ������1����һ�10����������
																													
	    set @ConsumptionOfGold=cast(@ExchangeNum/@CrushingDrillForGold as bigint)--�һ�������Ҫ�����ĵĽ������
		if @ConsumptionOfGold>@Moneybegin
		begin
			return 2     --������Ͻ�Ҳ���
		end
		--�һ�   ����������ϵĽ����������������
		update TUserInfo set WalletMoney=WalletMoney-@ConsumptionOfGold where UserID=@UserID
		update Web_Users set Lotteries=Lotteries+@ExchangeNum where UserID=@UserID
		select @Money=WalletMoney from TUserInfo where UserID=@UserID
		select @CrushingDrill=Lotteries from Web_Users where UserID=@UserID
		
		---����仯��־��
		INSERT INTO dbo.Web_LotteriesLog(UserID ,UserName ,LotteriesB ,ChangeLotteries ,LotteriesA ,InputDate ,Content ,ChangeType)
			VALUES  (@UserID, @UserName ,@CrushingDrillBegin ,@ExchangeNum ,@CrushingDrill,GETDATE(),'��ҽ�Ҷһ�����',6)
		
		select @Money as Money,@CrushingDrill as CrushingDrill
		return 0 
    end
END
GO

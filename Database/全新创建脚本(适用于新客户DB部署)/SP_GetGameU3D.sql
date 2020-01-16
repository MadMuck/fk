
set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go

-- =============================================
-- Author:		Fred Huang
-- Create date: 2008-11-18
-- Description:	��ȡ��Ϸ�Ƿ���U3D
-- =============================================
CREATE PROCEDURE [dbo].[SP_GetGameU3D]
(
	@NameID			INT				--��ϷID
)
AS
BEGIN

	SET NOCOUNT ON;
	
	DECLARE @RoomID INT 
	DECLARE @RoomRule INT 
	SET @RoomID=0
	
	SELECT TOP 1 @RoomID=RoomID FROM TGameRoomInfo WHERE GameNameID=@NameID
	
	IF 0=@RoomID
		RETURN 1
	
	SELECT @RoomRule=RoomRule FROM dbo.TGameRoomBase WHERE RoomID=@RoomID
	
	SELECT @RoomRule AS RoomRule

END

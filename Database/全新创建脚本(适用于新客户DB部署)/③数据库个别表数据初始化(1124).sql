/*初始化web.config表*/
INSERT INTO Web_Config(ID, BadUserName, BadNickName, MoneyToViptime, MoneyToViptimeDay, MoneyToViptimeWeek, MoneyToViptimeMonth, MoneyToViptimeHYear,
 MoneyToViptimeYear, UserobligateMoney, TransferLessMoney, TransferLessAllMoney, TransferTax, TransferMultiple, MaxTransTax, Con_ValidDay, Con_OnlineTime, 
 Con_ValidUserAddMoney, Con_CardInPercent, Con_GetMoneyPercent, Con_RegMoney, Con_ValidUserMoney, Con_MinClearingMoney, Con_MoneyChangeRate, RegWallet, 
 IPDisabled, ReRegTimeSpan, YeepayMsgInfo, VipTransferTax, Yeepay_Rate_NetBank, Yeepay_Rate_Junnet, Yeepay_Rate_Szx, Yeepay_Rate_Unicom, Yeepay_Rate_YPCard, 
 FreeCardCount, MaxTransUserMoney, StartPresent, EndPresent, CoinNum, TimeLength, GameNameID, TG_GameNameID,FineryIDDefaultPrice,FineryIDRegex) 
VALUES ('1','admin|user|services|superadmin|hujintao|maozedong|dengxiaoping','管理员|我是管理员|江泽民|胡锦涛|邓小平|毛泽东|超级管理员|客服|客服人员','2000',
'1000','700','3000','600000','1200000','10000','100000','10000','5','10','10000','100','100','100','100','10','1000','100','1000000','10000','10000',
default,default,default,default,default,default,default,default,default,default,'','','','','','','','1000',
'(\d)\1{2,}-(\d)\1{1,}(\d)\2{1,}-(?:0(?=1)|1(?=2)|2(?=3)|3(?=4)|4(?=5)|5(?=6)|6(?=7)|7(?=8)|8(?=9)){4}\d-(?:9(?=8)|8(?=7)|7(?=6)|6(?=5)|5(?=4)|4(?=3)|3(?=2)|2(?=1)|1(?=0)){4}\d-(([\d]){1,}([\d]){1,})\1{1,}-((\d){2,})\d\1{1}-(19|20)[\d]{2}((1[0,1,2])|(0[1,2,3,4,5,6,7,8,9]))((3[0,11])|(2[0,1,2,3,4,5,6,7,8,9])|(1[0,1,2,3,4,5,6,7,8,9])|(0[0,1,2,3,4,5,6,7,8,9]))')

/*初始化权限*/
insert into Web_Manage_Model values('01','首页FLASH管理','')
insert into Web_Manage_Model values('02','玩家留言管理','')
insert into Web_Manage_Model values('03','网站参数设置','')
insert into Web_Manage_Model values('04','用户管理中心','')
insert into Web_Manage_Model values('05','充值管理中心','')
insert into Web_Manage_Model values('06','游戏管理中心','')
insert into Web_Manage_Model values('08','转账管理中心','')
insert into Web_Manage_Model values('09','记录查询中心','')
insert into Web_Manage_Model values('10','新闻管理中心','')
insert into Web_Manage_Model values('11','奖品管理中心','')
insert into Web_Manage_Model values('12','系统配置中心','')
insert into Web_Manage_Model values('13','推广系统管理','')
insert into Web_Manage_Model values('14','靓号管理中心','')
insert into Web_Manage_Model values('16','魅力值兑换管理','')
insert into Web_Manage_Model values('15','道具管理中心','')
insert into Web_Manage_Model values('17','代理商帐号管理','')
insert into Web_Manage_Model values('18','发送信息管理中心','') 
insert into Web_Manage_Model values('19','比赛管理中心','') 
insert into Web_Manage_Model values('20','服装管理中心','') 

/*初始化新闻*/
declare @TypeId1 int;INSERT INTO Web_NewsType VALUES ('活动新闻','1');set @TypeId1=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId1,'最新活动','活动新闻','本站','admin','admin','活动新闻活动新闻','',1,0,0,getdate(),1)
declare @TypeId2 int;INSERT INTO Web_NewsType VALUES ('游戏新闻','6');set @TypeId2=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId2,'最新游戏','游戏新闻','本站','admin','admin','游戏新闻游戏新闻','',1,0,0,getdate(),1)
declare @TypeId3 int;INSERT INTO Web_NewsType VALUES ('系统新闻','7');set @TypeId3=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId3,'系统维护','系统新闻','本站','admin','admin','系统新闻系统新闻','',1,0,0,getdate(),1)
declare @TypeId4 int;INSERT INTO Web_NewsType VALUES ('推广系统','8');set @TypeId4=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId4,'推广员介绍','推广员介绍','本站','admin','admin','推广员介绍','',1,0,0,getdate(),1);
insert into Web_NewsData(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId4,'工作方式','工作方式','本站','admin','admin','工作方式','',1,0,0,getdate(),1);
insert into Web_NewsData(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId4,'业绩计算','业绩计算','本站','admin','admin','业绩计算','',1,0,0,getdate(),1);
insert into Web_NewsData(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId4,'提成条件','提成条件','本站','admin','admin','提成条件','',1,0,0,getdate(),1);
insert into Web_NewsData(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId4,'操作说明','操作说明','本站','admin','admin','操作说明','',1,0,0,getdate(),1)
declare @TypeId5 int;INSERT INTO Web_NewsType VALUES ('新手指南','9');set @TypeId5=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId5,'如何注册成为会员？','新手指南','本站','admin','admin','新手指南新手指南','',1,0,0,getdate(),1)

--6
declare @TypeId11 int;INSERT INTO Web_NewsType VALUES ('新闻动态','11');set @TypeId11=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId11,'新闻动态','新闻动态','本站','admin','admin','系统新闻系统新闻','',1,0,0,getdate(),1)

---7
declare @TypeId12 int;INSERT INTO Web_NewsType VALUES ('常见问题','12');set @TypeId12=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId12,'常见问题','常见问题','本站','admin','admin','系统新闻系统新闻','',1,0,0,getdate(),1)

--8
declare @TypeId10 int;INSERT INTO Web_NewsType VALUES ('公司简介','10');set @TypeId10=@@identity;insert into Web_NewsData
(News_Type,Title,Keywords,News_From,Author,Issuer,Content,News_picture,Through,Recommendation,IsTop,Issue_Time,Hits)values(@TypeId10,'公司简介','公司简介','本站','admin','admin','系统新闻系统新闻','',1,0,0,getdate(),1)

/*初始化道具*/
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('1','负分清零 ','1000','800','负分清零 ','16777218','16777216')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('2','送鲜花','10','8','送鲜花','33554513','16842752')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('3','拍砖头','10','8','拍砖头','50331761','33619968')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('4','小喇叭','150','100','小喇叭','67109121','67108864')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('5','大喇叭','200','160','大喇叭','83886209','83886209')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('6','1小时双倍积分卡','200','160','1小时双倍积分卡','100663300','100663297')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('7','3小时双倍积分卡','400','320','3小时双倍积分卡','117440516','117440515')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('8','防踢卡','200','160','防踢卡','134218241','134217728')
INSERT INTO [dbo].[TPropDefine]([PropID],[PropName],[Price],[VipPrice],[Descript],[ActionAttribute],[ValueAttribute])VALUES('9','踢人卡','400','320','踢人卡','150995969','150995004')

insert Web_ClearFasciPropDefine values('初级魅力值清零','初级魅力值清零：可以清零-1～-10个魅力值',3000,10,0,getdate()) 
insert Web_ClearFasciPropDefine values('中级魅力值清零','中级魅力值清零：可以清零-11～-50个魅力值',5000,30,0,getdate()) 
insert Web_ClearFasciPropDefine values('高级魅力值清零','高级魅力值清零：可以清零-51～-100个魅力值',8000,80,0,getdate())
insert dbo.Web_FasciExchangeConfig values(1,getdate(),getdate(),0,0,0)

/*初始化网站配置*/
insert into Web_System(ParaName,ParaValue,Descript)values('Title','','网站标题')
insert into Web_System(ParaName,ParaValue,Descript)values('ProductCopyright','','后台菜单-产品版权信息')
insert into Web_System(ParaName,ParaValue,Descript)values('copyright','','前台页脚版权信息')
insert into Web_System(ParaName,ParaValue,Descript)values('CustomServiceqq1','','客服QQ1')
insert into Web_System(ParaName,ParaValue,Descript)values('CustomServicePhone','','客服电话')
insert into Web_System(ParaName,ParaValue,Descript)values('UserQQColony1','','玩家QQ群1')
insert into Web_System(ParaName,ParaValue,Descript)values('SiteICP','','网站备案号')

insert into Web_System(ParaName,ParaValue,Descript)values('公司地址','','公司地址')
insert into Web_System(ParaName,ParaValue,Descript)values('文化经营许可证','','文化经营许可证')
insert into Web_System(ParaName,ParaValue,Descript)values('QQ1','','123456')
insert into Web_System(ParaName,ParaValue,Descript)values('QQ2','','123456')
insert into Web_System(ParaName,ParaValue,Descript)values('QQ3','','123456')
insert into Web_System(ParaName,ParaValue,Descript)values('客服电话','','客服电话')
insert into Web_System(ParaName,ParaValue,Descript)values('客服邮箱','','123@qq.com')
INSERT INTO Web_GameMain(Image_URL) VALUES ('/image/game/pic_dt.jpg')

/*初始化管理员*/
INSERT INTO Web_manage_admin VALUES ('admin','77804d2ba1922c33','1111111','2','127.0.0.1','2008-12-18 17:53:00','2003-09-25 23:13:00',
'745','3','3','3','','','u9pc72f664t5w7I4')

/*初始化代理*/
insert Web_Agency(AgencyName, AgencyPassword, ParentAgencyID, LevelID, TrueName, SpareValue, IsTaboo, LastLoginIP, DomainUrl)
values('admin','21218cca77804d2ba1922c33e0151105',0,1,'总公司',100,1,'192.168.0.211','game')
 
/*初始化服装*/
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('免费',0,10,0,1,0,0)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('免费',0,10,0,0,0,0)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('蜘蛛侠',50000,10,1,1,50000,100000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('不知火舞',50000,10,1,0,50000,100000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('蝙蝠侠',100000,10,2,1,100000,200000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('美少女战士',100000,10,2,0,100000,200000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('超人',150000,10,3,0,150000,300000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('靓丽天使',150000,10,3,0,150000,300000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('帝皇',200000,10,4,1,200000,300000)
INSERT INTO web_clothes(LogName,Price,SendLotteries,LogID,Sex,weekPrice,MonthPrice)VALUES('女王',200000,10,4,0,200000,300000)
/*运营系统配置*/
INSERT INTO dbo.Web_PConfig
        ( ID ,
          [Description] ,
          Field1 ,
          Field2 
       
        )
VALUES  ( 3, -- ID - int
          'Filed1:活跃用户标准（即当日所需的游戏局数）;Field2:网站名称' , -- Description - nvarchar(2000)
          '5' , -- Field1 - nvarchar(1000)
          '' 
        )

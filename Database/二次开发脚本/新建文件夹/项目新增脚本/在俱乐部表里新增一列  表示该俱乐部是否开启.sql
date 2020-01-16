alter table TClubTable add ClubIsOpen int not null default 1 with values;
---在俱乐部表里新增一列  表示该俱乐部是否开启  0---关闭  1----开启 （默认为开启）
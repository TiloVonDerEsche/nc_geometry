LOAD DATA INFILE '.\..\data\hmhis.csv'
INTO TABLE Test FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n' IGNORE 1 ROWS;

CREATE TABLE Test (
	tStmp ´datetime NOT NULL,
	LeftT varchar(20),
	RightT varchar(20),
	
	segment_id int(11) NOT NULL,
	segment_id int(11) NOT NULL,
	probability float NOT NULL,
	measured_at timestamp NOT NULL,
	provider_id int(11) NOT NULL,
	PRIMARY KEY (tStmp)	
); 

CREATE VIEW SuperTable AS
SELECT *
FROM Loan

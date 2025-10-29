CREATE TABLE hmhis (
    id   INT AUTO_INCREMENT PRIMARY KEY,
    payload JSON NOT NULL
);

/*
LOAD DATA INFILE 'E:/work/nc_geometry/data/hmhis.json'
INTO TABLE hmhis
CHARACTER SET utf8mb4
(@json)
SET payload = JSON_ARRAY_APPEND(
        JSON_ARRAY(), '$', JSON_EXTRACT(@json, '$[*]')
      );
*/

--path only works with forward slashes!
--check if file is found -> hex data or not -> NULL
SELECT LOAD_FILE('E:/work/nc_geometry/data/hmhis.json') AS file_content;


-- Step 1: Load file as binary
SET @json_bin = LOAD_FILE('E:/work/nc_geometry/data/hmhis.json');

-- Step 2: Convert to UTF-8 text
SET @json = CONVERT(@json_bin USING utf8mb4);

SELECT JSON_VALID(@json) AS is_valid, JSON_TYPE(@json) AS type;

-- Step 3: Now use JSON functions safely
INSERT INTO hmhis (payload)
WITH RECURSIVE idx AS (
    SELECT 0 AS n
    UNION ALL
    SELECT n + 1
    FROM idx
    WHERE n + 1 < JSON_LENGTH(@json)
)
SELECT JSON_EXTRACT(@json, CONCAT('$[', n, ']'))
FROM idx;

SELECT * FROM hmhis;

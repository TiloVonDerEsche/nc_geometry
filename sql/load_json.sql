DROP TABLE IF EXISTS hmhis;
CREATE TABLE hmhis (
    id   INT AUTO_INCREMENT PRIMARY KEY,
    payload JSON NOT NULL
);

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


-- -------------------------------------------------
-- 4. (Optional) Fast column + index
-- -------------------------------------------------
ALTER TABLE hmhis
ADD COLUMN vit_tir_num DECIMAL(12,6) GENERATED ALWAYS AS
       (JSON_UNQUOTE(JSON_EXTRACT(payload, '$.VIT_TIR')) + 0) STORED,
ADD INDEX idx_vit_tir (vit_tir_num);

-- -------------------------------------------------
-- 5. Example query
-- -------------------------------------------------
SELECT
    id,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.line'))    AS line,
    vit_tir_num                                      AS VIT_TIR
FROM   hmhis
WHERE  vit_tir_num >= 1750
ORDER  BY line;
------------
SELECT
    id,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.line'))      AS line,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.X'))      AS X,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.Y'))   AS Y,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.Z'))      AS Z,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.G'))   AS G,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser'))      AS laser,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser_power'))   AS laser_power,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.VIT_TIR'))   AS VIT_TIR
FROM hmhis
WHERE JSON_UNQUOTE(JSON_EXTRACT(payload, '$.VIT_TIR')) + 0 > 175
ORDER BY line;


-------------

SELECT
    id,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.line'))      AS line,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.X'))      AS X,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.Y'))   AS Y,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.Z'))      AS Z,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.G'))   AS G,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser'))      AS laser,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser_power'))   AS laser_power,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.VIT_TIR'))   AS VIT_TIR
FROM hmhis
WHERE JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser')) = 0
ORDER BY id;

--------------

SELECT
    id,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.line'))      AS line,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.X'))      AS X,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.Y'))   AS Y,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.Z'))      AS Z,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.G'))   AS G,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser'))      AS laser,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser_power'))   AS laser_power,
    JSON_UNQUOTE(JSON_EXTRACT(payload, '$.VIT_TIR'))   AS VIT_TIR
FROM hmhis
ORDER BY JSON_UNQUOTE(JSON_EXTRACT(payload, '$.laser'));


SELECT * FROM hmhis;

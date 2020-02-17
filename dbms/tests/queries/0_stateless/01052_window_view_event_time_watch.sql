SET allow_experimental_window_view = 1;

DROP TABLE IF EXISTS test.mt;

SELECT '--TUMBLE--';
DROP TABLE IF EXISTS test.wv;
CREATE TABLE test.mt(a Int32, timestamp DateTime) ENGINE=MergeTree ORDER BY tuple();
CREATE WINDOW VIEW test.wv WATERMARK INTERVAL '1' SECOND AS SELECT count(a) FROM test.mt GROUP BY TUMBLE(timestamp, INTERVAL '1' SECOND) AS wid;

INSERT INTO test.mt VALUES (1, now() + INTERVAL '1' SECOND);
WATCH test.wv LIMIT 1;

SELECT '--HOP--';
DROP TABLE IF EXISTS test.wv;
CREATE WINDOW VIEW test.wv WATERMARK INTERVAL '1' SECOND AS SELECT count(a) FROM test.mt GROUP BY HOP(timestamp, INTERVAL '1' SECOND, INTERVAL '2' SECOND) AS wid;

INSERT INTO test.mt VALUES (1, now() + INTERVAL '1' SECOND);
WATCH test.wv LIMIT 2;

SELECT '--INNER_TUMBLE--';
DROP TABLE IF EXISTS test.wv;
CREATE WINDOW VIEW test.wv ENGINE=MergeTree ORDER BY tuple() WATERMARK INTERVAL '1' SECOND AS SELECT count(a) FROM test.mt GROUP BY TUMBLE(timestamp, INTERVAL '1' SECOND) AS wid;

INSERT INTO test.mt VALUES (1, now() + INTERVAL '1' SECOND);
WATCH test.wv LIMIT 1;

SELECT '--INNER_HOP--';
DROP TABLE IF EXISTS test.wv;
CREATE WINDOW VIEW test.wv ENGINE=MergeTree ORDER BY tuple() WATERMARK INTERVAL '1' SECOND AS SELECT count(a) FROM test.mt GROUP BY HOP(timestamp, INTERVAL '1' SECOND, INTERVAL '2' SECOND) AS wid;

INSERT INTO test.mt VALUES (1, now() + INTERVAL '1' SECOND);
WATCH test.wv LIMIT 2;

DROP TABLE test.wv;
DROP TABLE test.mt;

USE sql_store;
SELECT 
	first_name, 
	last_name, 
    points % 10 AS modulo_ten,
    points % 5 AS 'modulo five',
	state
FROM customers;

SELECT DISTINCT state 
-- WHERE customer_id = 1
-- ORDER BY first_name; 
FROM customers;

-- Exercise 
SELECT 
	product_id,
	name, 
    unit_price AS 'unit price', 
    unit_price * 1.1 AS 'new price' 
FROM products
WHERE unit_price >= 3 AND product_id != '2';

SELECT *
FROM order_items
WHERE order_id = 6 OR unit_price * quantity >= 30;

SELECT *
FROM customers
WHERE state NOT IN ('VA', 'GA', 'FL');

SELECT *
FROM customers
WHERE points BETWEEN 1000 and 3000; -- Inclusive 

SELECT *
FROM customers
WHERE last_name LIKE '%b%' OR last_name LIKE '_____y'
ORDER BY state DESC, first_name;
-- % any number of characters
-- _ single character  

SELECT *, quantity * unit_price AS total_price
FROM order_items
WHERE order_id = 2
ORDER BY total_price DESC
-- LIMIT 2 
LIMIT 1, 2; -- LIMIT (#skip, #display)

SELECT order_id, first_name, last_name, orders.customer_id, customers.customer_id
FROM orders
JOIN customers 
	ON orders.customer_id = customers.customer_id;

SELECT order_id, first_name, last_name, o.customer_id, c.customer_id
FROM orders o
JOIN customers c
	ON o.customer_id = c.customer_id;
    
USE sql_store;
SELECT *
FROM order_items oi
JOIN sql_inventory.products p
	ON oi.product_id = p.product_id;
    
USE sql_hr;
SELECT 
	e.first_name AS employee_first_name, 
    e.last_name AS employee_last_name, 
    m.first_name AS manager_first_name, 
    m.last_name AS manager_last_name
FROM employees e
JOIN employees m
	ON e.reports_to = m.employee_id;

USE sql_store;
SELECT 
	o.order_id,
    o.order_date,
    c.first_name,
    c.last_name,
    os.name AS status
FROM orders o
JOIN customers c
	ON o.customer_id = c.customer_id
JOIN order_statuses os
	ON o.status = os.order_status_id;
    
SELECT *
FROM order_items oi
JOIN order_item_notes oin
	ON oi.order_id = oin.order_id
    AND oi.product_id = oin.product_id;

SELECT *
FROM orders o
JOIN customers c
	ON o.customer_id = c.customer_id; -- required

-- Implicit Join Syntac -- 
SELECT *
FROM orders o, customers c
WHERE o.customer_id = c.customer_id;

SELECT 
	c.customer_id, 
    c.first_name, 
    o.order_id
FROM customers c
LEFT JOIN orders o
	ON c.customer_id = o.customer_id
ORDER BY c.customer_id;

SELECT 
	c.customer_id,
    c.first_name,
    o.order_id,
    sh.name AS shipper
FROM customers c
LEFT JOIN orders o
	-- ON c.customer_id = o.customer_id
	USING (customer_id)
LEFT JOIN shippers sh
	ON o.shipper_id = sh.shipper_id
ORDER BY c.customer_id;

SELECT *
FROM order_items oi
JOIN order_item_notes oin
	-- ON oi.order_id = oin.order_id 
		-- AND oi.product_id = oin.product_id
	USING (order_id, product_id);

-- SELECT 
-- 	o.order_id,
--     c.first_name
-- FROM orders o
-- NATURAL JOIN customers c;

SELECT
	c.first_name AS customer,
    p.name AS product
FROM customers c
CROSS JOIN products p
ORDER BY c.first_name;

SELECT 
	o.order_id,
    o.order_date,
    o.status,
    'Active' AS status
FROM orders o
WHERE o.order_date >= '2019-01-01'
UNION
SELECT 
	o.order_id,
    o.order_date,
    o.status,
    'Archived' AS status
FROM orders o
WHERE o.order_date < '2019-01-01';

SELECT
	c.customer_id,
    c.first_name,
    c.points,
    'Gold' AS type
FROM customers c
WHERE c.points > 3000
UNION
SELECT
	c.customer_id,
    c.first_name,
    c.points,
    'Silver' AS type
FROM customers c
WHERE c.points BETWEEN 2000 and 3000
UNION
SELECT
	c.customer_id,
    c.first_name,
    c.points,
    'Bronze' AS type
FROM customers c
WHERE c.points < 2000
ORDER BY first_name;

-- INSERT INTO customers (
-- 	customer_id,
--     last_name,
-- 	first_name,
-- 	birth_date,
--     address,
--     city,
--     state
-- )
-- VALUE (
-- 	DEFAULT,
-- 	'Smith',
-- 	'John',  
--     '1990-01-01', 
--     'address',
--     'city',
--     'CA'
-- );

-- INSERT INTO shippers (name)
-- VALUES ('Shipper1'), ('Shipper2'), ('Shipper3');

-- CREATE TABLE order_archived
-- -- INSERT INTO order_archived
-- SELECT *
-- FROM orders o
-- WHERE o.order_date < '2019-01-01';

USE sql_invoicing;
UPDATE invoices
SET payment_total = 10, payment_date = '2019-03-01'
WHERE invoice_id = 1;

-- UPDATE invoices
-- SET payment_total = invoice_total * 0.5,
-- 	payment_date = due_date
-- WHERE client_id = (
-- 	SELECT client_id
-- 	FROM clients
-- 	WHERE name = 'Myworks'
-- );

-- UPDATE invoices
-- SET payment_total = invoice_total * 0.5,
-- 	payment_date = due_date
-- WHERE client_id IN (
-- 	SELECT client_id
-- 	FROM clients
-- 	WHERE state IN ('CA', 'NY')
-- );

USE sql_store;
UPDATE orders
SET comments = 'Gold customer'
WHERE customer_id IN (
	SELECT customer_id
	FROM customers
	WHERE points > 3000
);
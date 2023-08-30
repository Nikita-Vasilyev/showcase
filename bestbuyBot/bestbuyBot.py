import requests, json, time
from selenium import webdriver

# sets up options for chrome tab usage, finds driver file
options = webdriver.ChromeOptions() 
options.add_experimental_option("excludeSwitches", ["enable-logging"])
#options.add_argument("--incognito")
driver=webdriver.Chrome(options=options, executable_path=r'C:\Users\Nikita\Desktop\Python\chromedriver.exe')

# defines a function to determine if the item specified is in stock
def avaCheck(url):
    driver.get(str(url))
    try:
        driver.find_element_by_xpath('//button[@class="btn btn-primary btn-lg btn-block btn-leading-ficon add-to-cart-button"]')
        return True
    except:
        return False

# defines a function to enter the users shopping cart
def goToCart():
    index=0
    while True:
        try:
            driver.find_element_by_xpath('//div[@class="go-to-cart-button"]').click()
            break
        except:
            print('Cannot Add to Cart, Trying Again')
            try:
                driver.find_element_by_xpath('//button[@class="btn btn-primary btn-lg btn-block btn-leading-ficon add-to-cart-button"]').click()
                time.sleep(3)
            except:
                time.sleep(1)
            

# defines a function to go through with the purchase of the specified item
def buyItem(url):
    #Adding to cart
    driver.find_element_by_xpath('//button[@class="btn btn-primary btn-lg btn-block btn-leading-ficon add-to-cart-button"]').click()
    time.sleep(3)
    #Go to cart
    goToCart()
    time.sleep(3)
    #Checkout
    driver.find_element_by_xpath('//button[@data-track="Checkout - Top"]').click()
    time.sleep(3)
    #Enter username
    driver.find_element_by_xpath('//input[@type="email"]').send_keys('youremail@example.com')
    time.sleep(.2)
    #Enter password
    driver.find_element_by_xpath('//input[@type="password"]').send_keys('yourpasswordexample')
    time.sleep(.2)
    #Select sign in
    driver.find_element_by_xpath('//button[@data-track="Sign In"]').click()
    time.sleep(3)
    #First Name
    try:
        driver.find_element_by_xpath('//input[@id="consolidatedAddresses.ui_address_2.firstName"]').send_keys('First Name')#<><>CHANGE<><>
        time.sleep(.2)
    except:
        print('Press Enter Once Verification Code Has Been Entered')
        yes=input()
        driver.find_element_by_xpath('//input[@id="consolidatedAddresses.ui_address_2.firstName"]').send_keys('First Name')#<><>CHANGE<><>
        time.sleep(.2)
    #Last Name
    driver.find_element_by_xpath('//input[@id="consolidatedAddresses.ui_address_2.lastName"]').send_keys('Last Name')#<><>CHANGE<><>
    time.sleep(.2)
    #Address
    driver.find_element_by_xpath('//input[@id="consolidatedAddresses.ui_address_2.street"]').send_keys('Street Address')#<><>CHANGE<><>
    time.sleep(.2)
    driver.find_element_by_xpath('//input[@id="consolidatedAddresses.ui_address_2.city"]').send_keys('Town')#<><>CHANGE<><>
    time.sleep(.2)
    driver.find_element_by_xpath('//select[@id="consolidatedAddresses.ui_address_2.state"]').send_keys('State')#<><>CHANGE<><>
    time.sleep(.2)
    driver.find_element_by_xpath('//input[@id="consolidatedAddresses.ui_address_2.zipcode"]').send_keys('ZIP Code')#<><>CHANGE<><>
    time.sleep(.2)
    #Go to payment
    driver.find_element_by_xpath('//div[@class="button--continue"]').click()
    time.sleep(3)
    #Security Code
    driver.find_element_by_xpath('//input[@id="credit-card-cvv"]').send_keys('cvv')#<><>CHANGE<><>
    time.sleep(.2)
    #Place order
    driver.find_element_by_xpath('//button[@data-track="Place your Order - Contact Card"]').click()

# link to the item being tracked
print('Enter Link:')
url=input()

# checks an items avalibility until the item is in stock then, it is purchased
while True:
    if avaCheck(url):
        print('Item Available, attempting to purchase...')
        buyItem(url)
        break
    else:
        print('Not Available')
        time.sleep(30)

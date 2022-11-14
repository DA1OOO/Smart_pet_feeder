import sys
import os
import requests
import re
import urllib
from lxml import html

# 获取商品基本信息（除了价格）
def getInfo(url):
    kv = {'user-agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 11_2_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.128 Safari/537.36'}
    r = requests.get(url, headers=kv).content
    sel = html.fromstring(r)
    # 获取完整名称，text()获取该标签下的文本
    fullName = sel.xpath("//div[@class='sku-name']/text()")
    if fullName is not None:
        matchObj = re.match(r'\s*(.+)\s*', fullName[0])     #截取非空白字符
        if matchObj:
            fullName = matchObj.group(1)
    else:
        fullName = ''
    # 提取商家信息
    shop = sel.xpath("//*[@id='crumb-wrap']/div/div[2]/div[2]/div[1]/div/a/text()")[0]
    if shop is None:
        shop = ''
    # 获取图片地址
    imgUrl1 = sel.xpath("//*[@id='spec-n1']//img/@src")
    imgUrl2 = sel.xpath("//*[@id='spec-n1']//img/@data-origin")
    if(imgUrl1 == []):
        imgUrl = imgUrl2[0]
    elif(imgUrl2 == []):
        imgUrl = imgUrl1[0]
    else:
        imgUrl = ''

    if os.path.exists('jd_info.txt'):
        os.remove('jd_info.txt')
    with open(f'jd_info.txt','x', encoding='gbk') as text_file:
        text_file.write(fullName + '\n')
        text_file.write(shop + '\n')
        text_file.write(imgUrl + '\n')

def getPicture(html):
    reg = r'src="\/\/(.+?\.jpg)"'
    imgre = re.compile(reg)
    imglist = re.findall(imgre, html)
    x = 0
    for imgurl in imglist:
        x += 1
    return imglist


if __name__ == "__main__":
    url = sys.argv[1]
    getInfo(url)
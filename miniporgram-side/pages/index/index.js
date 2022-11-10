//index.js
//获取应用实例
const app = getApp()

Page({
  data: {
    inputShowed: false,
    inputVal: "",
    loading: false,
    hideLoading: false,
   
  },
  //登录成功提示
  onLoad: function(e){
    wx.showToast({
      title: '用户登录成功',
      duration: 2000,
    })
  },
  search: function (e) {
    this.setData({
      loading: true
    });
    //验证URL合法性
    let url = e.detail.value.url
    console.log(url)
    console.log(url.match(/\d+/g))//在输入完整url中取key
    //DA1YAYUAN push
    getApp().globalData.key = url.match(/\d+/g)

    let re = /(https:\/\/item.(.*)jd.com(\/[a-zA-Z])*\/[0-9]+.html).*/
    if(!re.test(url)) {
      this.setData({
        loading: false,
        hideLoading: false,
      });
      wx.showModal({
        title: '网址错误',
        content: '请确认是京东商品页链接',
      })
      return
    }

    url = url.replace(re, "$1")
    console.log(url)
    //存储查询网址
    if(url == wx.getStorageSync('url') && wx.getStorageSync('lastSearchSuccess')) {
      console.log("与上一次查询相同")
      this.setData({
        loading: false,
        hideLoading: false,
      });
      wx.navigateTo({
        url: '/pages/searchresult/searchresult'
      })
      wx.setStorageSync('lastSearchSuccess', true)
      return
    }
    wx.setStorageSync('url', url)

    app.post(
      'analyze',
      {
        openid: wx.getStorageSync('openid'),
        sessionKey: wx.getStorageSync('sessionKey'),
        url: url   //https://item.jd.com/100000287121.html
      }
    )
      .then(res => {
        let resData = res.data.data
        wx.setStorageSync('id', resData.id)   //商品全名
        wx.setStorageSync('itemName', resData.itemName)   //商品全名
        wx.setStorageSync('shopName', resData.shopName)   //商家名
        wx.setStorageSync('imgUrl', resData.imgUrl),  //商品图片URL
        //DA1YAYUAN push
        //wx.setStorageSync('ciyunUrl',resData.ciyunUrl),  //商品词云图
        wx.setStorageSync('commentsNum', resData.comments_num)    //分析评论数
        wx.setStorageSync('comments', resData.comments),        //部分评论
        wx.setStorageSync('rating', resData.average_sentiment)   //推荐指数

        this.setData({
          loading: false,
          hideLoading: false,
        });
        wx.navigateTo({
          url: '/pages/searchresult/searchresult'
        })
        wx.setStorageSync('lastSearchSuccess', true)
        return
      })
      .catch(data => {
        this.setData({
          loading: false,
          hideLoading: false,
        });
        console.log(data)
      })
    setTimeout(() => {
      if(this.data.loading) {
        this.setData({
          loading: false,
          hideLoading: false,
        });
        wx.showToast({
          title: '查询超时',
          icon: 'none'
        })
      }
    }, 300000);
  },

  showInput: function () {
    this.setData({
      inputShowed: true
    });
  },
  hideInput: function () {
    this.setData({
      inputVal: "",
      inputShowed: false
    });
  },
  clearInput: function () {
    this.setData({
      inputVal: ""
    });
  },
  inputTyping: function (e) {
    this.setData({
      inputVal: e.detail.value
    });
  },
  moreus:function(e){
    wx.navigateTo({
      url: '/pages/moreus/moreus'
    })
  }
})

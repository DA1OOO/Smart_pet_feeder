//pages/searchresult/searchresult.js
const app = getApp()

Page({

  /**
   * 页面的初始数据
   */
  data: {
    itemName: 'loading...',
    shopName: 'loading...',
    imgUrl: '',
    commentsNum: 'loading...',
    commentsIndex: [0, 1, 2, 3],
    comments: '',
    rating: 'loading...',
    price: ''
  },
// 情感分析直方图导航
  commentAnalysis(e){
    wx.navigateTo({
      url: '/pages/searchresult/pages/commentAnalysis/commentAnalysis',
    })
  },

  /**监听签到按钮是否被点击 */
  change: function () {
    var click = this.data.click;
    this.setData({
      isClick: true
    })

    let newNum = this.data.checkInNum + 1
    this.setData({
      checkInNum: newNum
    })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    var nowdate = new Date()
    this.setData({
      itemName: wx.getStorageSync('itemName'),
      shopName: wx.getStorageSync('shopName'),
      imgUrl: wx.getStorageSync('imgUrl'),
      
      //DA1YAYUAN push
      ciyunUrl: 'http://' + getApp().globalData.hostIp + ':8088/upload/' + getApp().globalData.key + '.jpg',
      //ciyunUrl:'/Users/daiyayuan/Desktop/IDEAWorkSpace/JD-analysis-wechat-serverside-master/src/main/resources/static/'+ getApp().globalData.key + '.jpg',
      commentsNum: wx.getStorageSync('commentsNum'),
      comments: wx.getStorageSync('comments'),
      rating: wx.getStorageSync('rating').toFixed(2)*100
    })

    //获取价格
    const that = this
    wx.request({
      url: 'https://p.3.cn/prices/mgets?skuIds=J_' + wx.getStorageSync('id'),
      success (res) {
        var price = res.data[0].p
        //console.log(res.data)
        that.setData({
          price: price
        })
      }
    })
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})
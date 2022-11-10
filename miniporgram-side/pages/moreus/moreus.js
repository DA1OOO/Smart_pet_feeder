Page({
  onShareAppMessage() {
    return {
      title: '我',
      path: 'moreus/moreus'
    }
  },

  data: {
    username: '', //微信用户昵称
    avatarUrl: '',  //微信用户头像图片链接地址
    list: [
      {
        id: 'user',
        name: '我的收藏',
        open: false,
        url: 'collections/collections'
      }
    ],
  },
  useInstruction(e){
    wx.navigateTo({
      url: '/pages/illustrate/illustrate',
    })
  },
  aboutUs(e){
    wx.navigateTo({
      url: '/pages/aboutus/aboutus',
    })
  },
  kindToggle(e) {
    const id = e.currentTarget.id; const
      list = this.data.list
    for (let i = 0, len = list.length; i < len; ++i) {
      if (list[i].id === id) {
        if (list[i].url) {
          wx.navigateTo({
            url: 'pages/' + list[i].url
          })
          return
        }
        list[i].open = !list[i].open
      } else {
        list[i].open = false
      }
    }
    this.setData({
      list
    })
  },

  onLoad() {
    var userInfo = wx.getStorageSync('userInfo')
    //console.log(userInfo)
    this.setData({
      username: userInfo.nickName,
      avatarUrl: userInfo.avatarUrl
    })
  },
})

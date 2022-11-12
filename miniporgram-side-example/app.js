//app.js

App({
  //POST请求封装
  post: function (uri, data) {
    var promise = new Promise((resolve, reject) => {
      var postData = data
      wx.request({
        url: 'http://' + this.globalData.hostIp + ':8088/wechat/' + uri,
        data: postData,
        header: {
          'content-type': 'application/x-www-form-urlencoded'
        },
        method: 'POST',
        success: function (res) {
          console.log(res)
          switch(res.data.status) {
            case 200:
              console.log('Http post \\' + uri + ' 成功')
              resolve(res)
              break;
            case 500:
              wx.showToast({
                icon: 'none',
                title: '服务器无响应，请稍后重试(500)',
                duration: 3000
              })
              reject(res.data)
              break;
            case 501:
              wx.showToast({
                icon: 'none',
                title: '用户信息错误，请重试(501)',
                duration: 3000
              })
              reject(res.data)
              break;
            case 502:
              wx.showToast({
                icon: 'none',
                title: '用户登录态错误，请重试(502)',
                duration: 3000
              })
              reject(res.data)
              break;
            default:
              wx.showToast({
                icon: 'none',
                title: '发生未知错误，请稍后重试(' + res.statusCode + ')',
                duration: 3000
              })
              reject(res.data)
              console.log('错误信息:') //打印错误信息
              console.log(res.data)
          }
        },
        fail: function (e) {
          console.log('网络出错，请重试')
          reject(e)
        }
      })
    })
    return promise
  },

  //微信小程序启动执行的代码
  onLaunch: function () {
    /* 获取用户信息，必须是在用户已经授权的情况下调用
     * 2020.3.9:腾讯已经修改了获取用户开放数据的方式，这个方法应该被弃用
     */
    // wx.getUserInfo({
    //   success: function (res) {
    //     console.log('获取用户信息成功')
    //     wx.setStorageSync('rawData', res.rawData)
    //     console.log(res.rawData)  //调试：显示rawData
    //     wx.setStorageSync('userInfo', res.userInfo)
    //     console.log(res.userInfo)  //调试：显示用户数据
    //     wx.setStorageSync('signature', res.signature)
    //     wx.setStorageSync('encryptedData', res.encryptedData)
    //     wx.setStorageSync('iv', res.iv)
    //   },
    //   fail: function (res) {
    //     wx.showToast({
    //       title: '获取用户信息失败',
    //       icon: 'none'
    //     })
    //     console.log(res)
    //   }
    // })

    // 微信主体登录
    wx.login({
      /* 登录成功，微信服务器返回 code 为用户登录凭证（有效期五分钟）。
       * 开发者需要在开发者服务器后台调用 auth.code2Session
       * 使用 code 换取 openid 和 session_key 等信息
       */
      success: (res) => {
        if (res.code) {
          /**
           *  发送 res.code 到后台换取 openId, sessionKey, unionId
           *  调用封装后的wx.request请求
           */
          this.post(
            'weLogin',
            {
              code: res.code
            }
          )
            .then(res => {
              //请求成功
              //console.log('openid: ' + res.data.data.openid)
              //console.log('sessionKey: ' + res.data.data.sessionKey)
              //将请求返回的结果存储下来
              wx.setStorageSync('openid', res.data.data.openid)
              wx.setStorageSync('sessionKey', res.data.data.sessionKey)
            })
            .catch(data => {
              console.log(data)
            })
        } else {
          console("获取用户态登录失败:" + res.errMsg);
        }
      },
      fail: (res) => {
        wx.showToast({
          icon: 'none',
          title: '登录失败，请重试',
        })
      }
    })

    wx.setStorageSync('lastSearchSuccess', false)
  },



  //全局数据
  globalData: {
    /**
     * 同一局域网改为192.168.137.1
     * 生产环境改为101.132.72.106
     */
    hostIp: 'localhost',
    key: '100008177989'
  },
})
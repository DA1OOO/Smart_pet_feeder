import com.da1yayuan.wechatserverside.util.PythonRunnerUtil;
import lombok.extern.slf4j.Slf4j;
import org.junit.Test;

import java.util.concurrent.CountDownLatch;
@Slf4j
public class PythonSpiderTest {

    @Test
    public void pythonSpiderTest(){
        String url = "https://item.jd.com/100000205012.html";
        String openId = "oY1mB4g3MhuTNLA1de7JuRFCKZ0E";
        String sourcePath = System.getProperty("user.dir") + "/python-spider";
        CountDownLatch latch = new CountDownLatch(2);
        Thread th = new Thread(new PythonRunnerUtil(sourcePath, url, openId, "jd_comment.py", latch));
        Thread th2 = new Thread(new PythonRunnerUtil(sourcePath, url, openId, "jd_info.py", latch));
        th.start();
        th2.start();
        try {
            latch.await();
        } catch (InterruptedException e) {
            log.warn(String.valueOf(e));
        }
    }
}

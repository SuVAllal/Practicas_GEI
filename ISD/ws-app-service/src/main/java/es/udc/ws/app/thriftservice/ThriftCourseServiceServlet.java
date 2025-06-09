package es.udc.ws.app.thriftservice;

import es.udc.ws.courses.thrift.ThriftCourseService;
import es.udc.ws.util.servlet.ThriftHttpServletTemplate;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocolFactory;

public class ThriftCourseServiceServlet extends ThriftHttpServletTemplate {
    public ThriftCourseServiceServlet() {
        super(createProcessor(), createProtocolFactory());
    }
    private static TProcessor createProcessor() {
        return new ThriftCourseService.Processor<ThriftCourseService.Iface>(new ThriftCourseServiceImpl());
    }

    private static TProtocolFactory createProtocolFactory() {
        return new TBinaryProtocol.Factory();
    }
}
